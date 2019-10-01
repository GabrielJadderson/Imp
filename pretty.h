#include "tree.h"

void pretty_print(AST_NODE* AST);
void prettyDIRECTIVE_LIST(DIRECTIVE_LIST* directive_list);
void prettyDIRECTIVE(DIRECTIVE* directive);
void prettyAST(AST_NODE* AST);
void prettySTRING(STRING* s);
void prettyFUNCTION(FUNCTION* f);
void prettyHEAD(HEAD* head);
void prettyTYPE(TYPE* type);
void prettyPAR_DECL_LIST(PAR_DECL_LIST* par_decl_list);
void prettyVAR_DECL_LIST(VAR_DECL_LIST* var_decl_list);
void prettyVAR_TYPE(VAR_TYPE* var_type);
void prettyBODY(BODY* body);
//void prettyBODY_DECL_LIST(BODY_DECL_LIST* body_decl_list);
void prettyDECL_LIST(DECL_LIST* decl_list);
void prettyDECL(DECL* decl);
void prettySTM_LIST(STM_LIST* stm_list);
void prettySTM(STM* stm);
void prettyVARIABLE(VARIABLE* var);
void prettyEXP(EXP* exp);
void prettyTERM(TERM* term);
void prettyACT_LIST(ACT_LIST* act_list);
void prettyEXP_LIST(EXP_LIST * exp_list);
