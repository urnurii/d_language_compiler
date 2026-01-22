#ifndef AST_BUILDER_H
#define AST_BUILDER_H

#include "tree_nodes.h"

// ----- Функции для типов -----

NType* CreateBaseType(BaseType base_type);
NType* CreateClassType(char *class_name);
NArrayDecl* CreateArrayDecl(int has_size, int size);
NType* AddArrayToType(NType *type, NArrayDecl *array);

// ----- Функции для инициализаторов -----

NInitializer* CreateExprInitializer(NExpr *expr);
NInitializer* CreateArrayInitializer(NExpr **exprs, int count);

// ----- Функции для выражений - литералы -----

NExpr* CreateIdentExpr(const char *name);
NExpr* CreateIntExpr(int value);
NExpr* CreateFloatExpr(double value);
NExpr* CreateCharExpr(char value);
NExpr* CreateStringExpr(const char *value);
NExpr* CreateBoolExpr(int value);
NExpr* CreateNullExpr(void);
NExpr* CreateNanExpr(void);
NExpr* CreateThisExpr(void);

// ----- Функции для выражений - операции -----

NExpr* CreateParenExpr(NExpr *expr);
NExpr* CreateCastExpr(NType *target_type, NExpr *expr);
NExpr* CreateUnaryOpExpr(OpType op, NExpr *operand);
NExpr* CreateBinaryOpExpr(OpType op, NExpr *left, NExpr *right);
NExpr* CreateAssignExpr(OpType op, NExpr *left, NExpr *right);

// ----- Функции для выражений - доступ и вызовы -----

NExpr* CreateArrayAccessExpr(NExpr *array, NExpr *index, NExpr *index_end);
NExpr* CreateMemberAccessExpr(NExpr *object, const char *member_name);
NExpr* CreateMethodCallExpr(NExpr *object, const char *method_name, NExpr **args, int arg_count);
NExpr* CreateFuncCallExpr(const char *func_name, NExpr **args, int arg_count);
NExpr* CreateNewExpr(NType *type, NExpr **args, int arg_count);
NExpr* CreateSuperExpr(const char *member_name);
NExpr* CreateSuperMethodCallExpr(const char *method_name, NExpr **args, int arg_count);

// ----- Функции для управления списками выражений ------

NExprList* CreateExprList(void);
void AddExprToList(NExprList *list, NExpr *expr);
void FreeExprList(NExprList *list);

// ----- Функции для параметров -----

NParam* CreateParam(NType *param_type, const char *param_name, 
                   int is_ref, NExpr *default_value);

NParamList* CreateParamList(void);
void AddParamToList(NParamList *list, NParam *param);
void FreeParamList(NParamList *list);

// ----- Функции для объявлений переменных -----

NInitDecl* CreateInitDecl(const char *name, NInitializer *initializer);

NInitDeclList* CreateInitDeclList(void);
void AddInitDeclToList(NInitDeclList *list, NInitDecl *decl);
void FreeInitDeclList(NInitDeclList *list);

// ----- Функции для операторов -----

NStmt* CreateExprStmt(NExpr *expr);
NStmt* CreateDeclStmt(NType *decl_type, NInitDeclList *init_decls);
NStmt* CreateCompoundStmt(NStmtList *stmt_list);
NStmt* CreateReturnStmt(NExpr *return_expr);
NStmt* CreateBreakStmt(void);
NStmt* CreateContinueStmt(void);

// ----- Функции для управления потоком -----

NStmt* CreateIfStmt(NExpr *condition, NStmt *then_stmt, NStmt *else_stmt);
NStmt* CreateWhileStmt(NExpr *condition, NStmt *body);
NStmt* CreateDoWhileStmt(NStmt *body, NExpr *condition);
NStmt* CreateForStmt(NExpr *init_expr, NType *init_decl_type, NInitDeclList *init_decls,
                    NExpr *cond_expr, NExpr *iter_expr, NStmt *body);
NStmt* CreateForeachStmt(int is_typed, NType *var_type, const char *var_name,
                        NExpr *collection, NStmt *body);
NStmt* CreateSwitchStmt(NExpr *switch_expr, NCaseItem **cases, int case_count);

// ----- Функции для управления списками операторов -----

NStmtList* CreateStmtList(void);
void AddStmtToList(NStmtList *list, NStmt *stmt);
void FreeStmtList(NStmtList *list);
NCaseItem* CreateCaseItem(NExpr *case_expr, NStmtList *stmts);
NCaseItem* CreateDefaultItem(NStmtList *stmts);
NCaseList* CreateCaseList(void);
void AddCaseItemToList(NCaseList *list, NCaseItem *item);
void FreeCaseList(NCaseList *list);

// ----- Функции для функций -----

NFuncDef* CreateFuncDef(NType *return_type, const char *func_name,
                       NParamList *params, NStmt *body);

NFuncDef* CreateVoidFuncDef(const char *func_name, NParamList *params, NStmt *body);

// ----- Функции для методов -----

NMethodDef* CreateMethodDef(int is_override, NType *return_type, const char *method_name,
                           NParamList *params, NStmt *body);

NMethodDef* CreateVoidMethodDef(int is_override, const char *method_name,
                               NParamList *params, NStmt *body);

NCtorDef* CreateCtorDef(NParamList *params, NStmt *body);
NDtorDef* CreateDtorDef(NStmt *body);

// ----- Функции для членов класса -----

NClassMember* CreateFieldMember(AccessSpec access, NType *field_type, NInitDeclList *init_decls);
NClassMember* CreateMethodMember(AccessSpec access, NMethodDef *method);
NClassMember* CreateCtorMember(AccessSpec access, NCtorDef *ctor);
NClassMember* CreateDtorMember(AccessSpec access, NDtorDef *dtor);
NClassMember* CreateEnumMember(AccessSpec access, NEnumDef *enum_def);
NClassMember* AppendClassMember(NClassMember *list, NClassMember *member);

// ----- Функции для классов -----

NClassDef* CreateClassDef(const char *class_name, const char *base_class_name,
                         NClassMember *first_member);

// ----- Функции для enum -----

NEnumItem* CreateEnumItem(const char *name, int has_value, int value);
NEnumDef* CreateEnumDef(const char *enum_name, NEnumItem **items, int item_count);
NEnumItemList* CreateEnumItemList(void);
void AddEnumItemToList(NEnumItemList *list, NEnumItem *item);

// ----- Функции для программы -----

NSourceItem* CreateFuncSourceItem(NFuncDef *func);
NSourceItem* CreateClassSourceItem(NClassDef *class_def);
NSourceItem* CreateDeclSourceItem(NType *item_type, NInitDeclList *init_decls);
NSourceItem* CreateEnumSourceItem(NEnumDef *enum_def);
NProgram* CreateProgram(NSourceItem *first_item);
void AddSourceItemToProgram(NProgram *prog, NSourceItem *item);

// ----- Вспомогательные функции для работы с памятью -----

char* DuplicateString(const char *str);

// Освобождает всё дерево (от корня вниз)
void FreeProgram(NProgram *prog);
void FreeSourceItem(NSourceItem *item);
void FreeStmt(NStmt *stmt);
void FreeExpr(NExpr *expr);
void FreeType(NType *type);

#endif
