//I pledge my Honor that I have not cheated, and will not cheat, on this assignment.--SUDHAMSH REDDY DOKURU
#include <typecheck.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
int isSubtype(int sub, int super) {//0==>is super is not ancestor of super else returns 1
    if(sub<-3||super<-3){//undefiend class(base condition)
        printf("Invalid class accessed/created");
        exit(-1);
        return 0;
    }
    if(super==sub){//A is ancestor of itself 
        return 1;
    }
    if(super==0){//Object is super class of every class
        return 1;
    }
    if(sub==-2){//Null is sub class of every class
        return 1;
    }
    //cycle check and hierarchy check;
    //A->B,b->C==>A is subtype of C
    bool check[numClasses];
    int temp = sub;
    while(temp>0&&temp!=super){
        if(check[temp]){//cycle condition
            printf("Invalid class hierarchy cycly found");
            exit(-1);
        }
        check[temp]=true;
        temp = classesST[sub].superclass;
    }
    if(temp==super){//sub is subtype of super
        return 1;
    }
    if(temp<-3){
        printf("Invalid class accessed/created");
        exit(-1);
    }
    // if(temp==sub){// covered in while loop
    //     printf("Invalid class hierarchy cycly found");
    //     exit(-1);
    // }
    return 0;//sub is sub class of super
    
}


int typeExpr(ASTree *t, int classContainingExpr, int methodContainingExpr){
    switch (t->typ)
    {
        case NAT_LITERAL_EXPR:
            return -1;//return -1 if exp is nat
        case NULL_EXPR:
            return -2;//return -2 if exp is null type
        case READ_EXPR: 
            return -1;//return -1 because read function returns nat(-1)
        case PLUS_EXPR:
            int x = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);//type of left side of plus it should be nat
            if(x!=-1||x!=typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr)){//check if right side of plus is of type nat or not
                printf("Invalid type not nat during PLUS opperation in line number:%d",t->lineNumber);//prints the line number and error details
                exit(-1);
            }
            return x;//returns -1
            break;
        case MINUS_EXPR://same as above
            int x = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);//type of left side of minus it should be nat
            if(x!=-1||x!=typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr)){//check if right side of plus is of type nat or not
                printf("Invalid type not nat during MINUS opperation in line number:%d",t->lineNumber);//prints the line number and error details
                exit(-1);
            }
            return x;//returns -1
            break;
        case TIMES_EXPR://same as plus and minus
            int x = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);//type of left side of minus it should be nat
            if(x!=-1||x!=typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr)){//check if right side of plus is of type nat or not
                printf("Invalid type not nat during Multiplication opperation in line number:%d",t->lineNumber);//prints the line number and error details
                exit(-1);
            }
            return x;//returns -1
            break;
        case PRINT_EXPR:
            if(typeExpr(t->children->data, classContainingExpr, methodContainingExpr) == -1) //print exp accepts only nat type
                return -1;
            printf("Invalid type printNat accepts only nat type error in line number:%d",t->lineNumber);
            exit(-1);
            break;
        case EQUALITY_EXPR:// same as PLUS 
            int x = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);//type of left side of == it should be nat(type(a)==type(b)==nat type)
            if(x!=typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr)){//check if right side of == is of type nat or not
                printf("Invalid type not nat during EQUALITY expression opperation in line number:%d",t->lineNumber);//prints the line number and error details
                exit(-1);
            }
        case GREATER_THAN_EXPR://same as EQUALITY_EXPR
            int x = typeExpr(t->children->data, classContainingExpr, methodContainingExpr);//type of left side of == it should be nat(type(a)==type(b)==nat type)
            if(x!=-1||x!=typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr)){//check if right side of == is of type nat or not
                printf("Invalid type not nat during EQUALITY expression opperation in line number:%d",t->lineNumber);//prints the line number and error details
                exit(-1);
            }
        
            return x;//returns -1
            break;
        case ASSIGN_EXPR://left handside should be sub type of right handside
            int left =  typeExpr(t->children->data, classContainingExpr, methodContainingExpr);
            int right = typeExpr(t->children->next->data, classContainingExpr, methodContainingExpr);
            if(isSubtype(left,right))
                return left;
            printf("Incompatable type error in line number:%d",t->lineNumber);
            exit(-1);
        case NOT_EXPR://accepts only nat 
            if(typeExpr(t->children->data, classContainingExpr, methodContainingExpr) == -1) //not exp accepts only nat type 0 or non 0
                return -1;
            printf("Invalid type NOT Expression accepts only nat type error in line number:%d",t->lineNumber);
            exit(-1);
            break;
        case AND_EXPR://accepts only nat
            if(typeExpr(t->children->data, classContainingExpr, methodContainingExpr) == -1) //and exp accepts only nat type 0 or non 0
                return -1;
            printf("Invalid type NOT Expression accepts only nat type error in line number:%d",t->lineNumber);
            exit(-1);
            break;
        case EXPR_LIST:
            return typeExprs(t->data,classContainingExpr,methodContainingExpr);
        case ARG_LIST://similar to EXPR_LIST but in loop
            ASTList *temp = t->children;
            while(temp!=NULL&&temp->data!=NULL){
                typeExprs(temp->data,classContainingExpr,methodContainingExpr);
                temp=temp->next;
            }
            return -1;//returns nat values
        case NEW_EXPR:
            int x = -7;// x stores id of class
            for(int i=0;i<numClasses&&x==-7;i++){//numClassess== number of classes in code
                if(strcmp(classesST[i].className,t->children->data->idVal)==0){//classesST array of class details
                    x = i;
                }
            }
            if(x>=0){//0 for Object object and {1,2,3,4,....} for user defined classes
                return x;
            }
            printf("Invalid Class declaration in line number:%d",t->lineNumber);//invalid class declartion error
            exit(-1);
            break;
        case WHILE_EXPR:
            int natType =-1;
            if(typeExpr(t->children->data,classContainingExpr,methodContainingExpr)!=natType){
                printf("Invalid while condition(should return nat value) in line number:%d",t->lineNumber);//invalid while condition
                exit(-1);
            }
            typeExprs(t->children->next->data,classContainingExpr,methodContainingExpr);//checking for type errors in body;
            return natType;
        case IF_THEN_ELSE_EXPR://similar to while
            int natType =-1;
            if(typeExpr(t->children->data,classContainingExpr,methodContainingExpr)!=natType){
                printf("Invalid if condition(should return nat value) in line number:%d",t->lineNumber);//invalid while condition
                exit(-1);
            }
            typeExprs(t->children->next->data,classContainingExpr,methodContainingExpr);//checking for type errors in then body;
            typeExprs(t->children->next->next->data,classContainingExpr,methodContainingExpr);//checking for type errors in else body;
            return natType;
        case THIS_EXPR:
            if(classContainingExpr==-1||methodContainingExpr==-1)//-1 refers to main method
            {
                printf("Invalid usage of 'this' keyword in line number:%d",t->lineNumber);
                exit(-1);
            }
            return classContainingExpr;
        case DOT_ID_EXPR://E.id
            return -1;//need to implement
        case ID_EXPR:/* ID */
            return -1;//need to implement
        case DOT_ASSIGN_EXPR:
            return -1;//need to implement
        case METHOD_CALL_EXPR:
            return -1;//need to implement
        case AST_ID:
            return -1;//need to implement
    default:
        printf("Invalid Type error in line number:%d",t->lineNumber);//Default error
        exit(-1);
        break;
    }
    return -1;//base condition
}

int typeExprs(ASTree *t, int classContainingExprs, int methodContainingExprs){
    if(t->children==NULL){
        return -2;//base condition returns any object type NULL
    }
    int currentType,preType;

    t = t->children;//t<==list of expressions
    preType = typeExpr(t->data,classContainingExprs,methodContainingExprs);//returns the type of the expression
    t=t->next;//t<==next expression
    while(t!=NULL){
        currentType = typeExpr(t->data,classContainingExprs,methodContainingExprs);
        // if(currentType!=preType){
        //     printf("Incompable type Error in line number : %d",&(t->lineNumber));
        //     exit(-1);
        // }
        preType = currentType;
        t=t->next;//t<==next expression
    }
    return preType;
}
void typecheckProgram() {
    typeExprs(mainExprs,-1,-1);//program exicution starts from main method
    //-1==>main method
}

