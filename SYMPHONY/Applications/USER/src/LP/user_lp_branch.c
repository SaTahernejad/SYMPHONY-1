/*===========================================================================*/
/*                                                                           */
/* This file is part of the SYMPHONY Branch, Cut, and Price Library.         */
/*                                                                           */
/* SYMPHONY was jointly developed by Ted Ralphs (ted@lehigh.edu) and         */
/* Laci Ladanyi (ladanyi@us.ibm.com).                                        */
/*                                                                           */
/* (c) Copyright 2000-2007 Ted Ralphs. All Rights Reserved.                  */
/*                                                                           */
/* This software is licensed under the Eclipse Public License. Please see    */
/* accompanying file for terms.                                              */
/*                                                                           */
/*===========================================================================*/

/* SYMPHONY include files */
#include "sym_constants.h"
#include "sym_macros.h"
#include "sym_lp_u.h"

/* USER include files */
#include "user.h"

/*===========================================================================*/

/*===========================================================================*\
 * This file contains the user-written functions for the LP process related
 * to branching.
\*===========================================================================*/

/*===========================================================================*\
 * This function determines whether to branch. You can essentially
 * leave this up to SYMPHONY unless there is some compelling reason not to.
\*===========================================================================*/

int user_shall_we_branch(void *user, double lpetol, int cutnum,
			 int slacks_in_matrix_num, cut_data **slacks_im_matrix,
			 int slack_cut_num, cut_data **slack_cuts, int varnum,
			 var_desc **vars, double *x, char *status,
			 int *cand_num, branch_obj ***candidates,
			 int *action)
{
   user_problem * prob = (user_problem *)user;

   if (prob->feasible == IP_INFEASIBLE) {
      *action = USER__DO_BRANCH;
   } else {
      *action = USER__DO_NOT_BRANCH;
   }
   return(USER_SUCCESS);
}

/*===========================================================================*/

/*===========================================================================*\
 * Here, we select the branching candidates. This can essentially be
 * left to SYMPHONY too using one of the built-in functions, but here, I
 * demonstrate how to branch on cuts, which must be done by the user.
\*===========================================================================*/

int user_select_candidates(void *user, double lpetol, int cutnum,
			   int slacks_in_matrix_num,
			   cut_data **slacks_in_matrix, int slack_cut_num,
			   cut_data **slack_cuts, int varnum, var_desc **vars,
			   double *x, char *status, int *cand_num,
			   branch_obj ***candidates, int *action,
			   int bc_level)

{
   user_problem *prob = (user_problem *)user;
   branch_obj *cand;
   int cind;

   *cand_num = 1; /* TODO: Change this to prob->vvnum, and the following code accordingly */

   *candidates = (branch_obj **) malloc(*cand_num * sizeof(branch_obj *));
   cand = (*candidates)[0] = (branch_obj *) calloc(1, sizeof(branch_obj));

   /*
   for (i = 0; i < prob->vvnum; i++) {
      if (prob->ccind[prob->vvind[i]] >= 0) {
         cind = prob->ccind[prob->vvind[i]];
         break;
      }
   }
   */
   cind = prob->ccind[prob->vvind[0]];

   cand->child_num = 2;
   cand->cdesc[0].type = CANDIDATE_CUT_IN_MATRIX;
   cand->cdesc[1].type = CANDIDATE_VARIABLE;

   cand->cdesc[0].position = cind;
   cand->cdesc[1].position = prob->vvind[0];
   
   cand->cdesc[0].sense = 'E';
   cand->cdesc[1].sense = 'E';
   
   if (prob->mip->sense[cind] == 'G') {
      cand->cdesc[0].rhs = -1.0 * prob->mip->rhs[cind];
   } else {
      cand->cdesc[0].rhs = prob->mip->rhs[cind];
   }
   cand->cdesc[1].rhs = 0;
   
   cand->cdesc[0].range = cand->cdesc[1].range = 0;

   return(USER_SUCCESS);
}

/*===========================================================================*/

int user_compare_candidates(void *user, branch_obj *can1, branch_obj *can2,
			    double ub, double granularity,
			    int *which_is_better)
{
   *which_is_better = SECOND_CANDIDATE_BETTER_AND_BRANCH_ON_IT;
   return(USER_SUCCESS);
}

/*===========================================================================*/

/*===========================================================================*\
 * You can let SYMPHONY choose which child to retain. The default is
 * to keep the one with the lower objective function value.
\*===========================================================================*/

int user_select_child(void *user, double ub, branch_obj *can, char *action)
{
   return(USER_DEFAULT);
}

/*===========================================================================*/

/*===========================================================================*\
 * Here, you can print out a more identifiable description of the
 * branching object than just "variable 51". 
\*===========================================================================*/

int user_print_branch_stat(void *user, branch_obj *can, cut_data *cut,
			   int n, var_desc **vars, char *action)
{
   return(USER_DEFAULT);
}
