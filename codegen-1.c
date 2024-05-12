//I pledge my Honor that I have not cheated, and will not cheat, on this assignment.--SUDHAMSH REDDY DOKURU
//Implemented till Level2 partially implemented level 3 
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include "codegen.h"
#include "symtbl.h"
/*R[0]<=0;R[7]=FramePointer;R[6]=StackPointer;R[5]=HeapPointer;(R[1],R[2],R[3])=temporary pointers*/
/*DECLARING GLOBAL VARIABLES:*/
int MAX ,fp,sp,hp,labelNumber;
void codeGenExpressions(ASTList *exp,FILE *fp,int classContainingExprs,int methodContainingExprs);
void decSp(FILE *fp);
void genPrologue(FILE *fp){//need to update heap pointer
    int i=1;
    fprintf(fp,";Prologue for all methods starts here\n");
    while(i<numClasses){
        int j=0;
        while(j<classesST[i].numMethods){
            int k=0;
            while(k<classesST[i].methodList[j].numLocals){
                fprintf(fp, "str 6 0 0;\t data of class:%d ; method:%d ; varaible:%d\n",i,j,k);
                decSp(fp);
                k++;
            }
            j++;
        }
        i++;
    
    }
    return ;
}
void genEpilogue(FILE *fp){
    //need to implement
    return;
}
void generateVtable(FILE *fp){

    //need to implement
    return ;
}
void initializeRegisters(FILE *outputFile){
    // initializing the variables and registers
    MAX=65535;
    fp=MAX;
    sp=MAX;
    hp=1;
    fprintf(outputFile,"mov 0 0\t;storing value 0 in register 0\n");
    fprintf(outputFile,"mov 7 %d\t;storing framepointer value into R[7]\n",fp);
    fprintf(outputFile,"mov 6 %d\t;storing stackpointer value into R[6]\n",sp);
    fprintf(outputFile,"mov 5 %d\t;storing heappointer value into R[5]\n",hp);
    return;
}
void decSp(FILE *fp){
    int error ;
    error = labelNumber++;
    sp--;
    fprintf(fp,"mov 1 1\t;storing value one in R[1]\n");
    fprintf(fp,"sub 6 6 1\t;decrementing stack pointer\n");
    fprintf(fp,"bgt 6 5 #NotErrorCode%d\t;if stack pointer is greater than heap the program halts\n",error);
    fprintf(fp,"mov 1 77\t;error code for stack heap collision\n");
    fprintf(fp,"hlt 1\t;(HP>=SP)\n");
    fprintf(fp,"#NotErrorCode%d:mov 0 0\n",error);
}
void incSp(FILE *fp){
    sp++;
    fprintf(fp,"mov 1 1\t;storing value one in R[1]\n");
    fprintf(fp,"add 6 6 1\t;incrementing the stack pointer\n");
}
int countExpressions(ASTList* x){//returns number of expression in a expression list    
    int ans;
    while(x!=NULL&&x->next!=NULL){
        ans++;
        x=x->next;
    }
    return ans;
}
int getMainIDNumber(char* name){
    int value=-7;
    int i=0;
    for(i=0;i<numMainBlockLocals;i++){
        if(!strcmp(name,mainBlockST[i].varName))
            return i;
    }
    return value;
}
int getVariableIndex(char* name,int classId,int methodId){
    int i;
    for(i =0;i<classesST[classId].methodList[methodId].numLocals;i++){
        if(!strcmp(classesST[classId].methodList[methodId].localST[i].varName,name))
            return i;
    }
    return -1;
}
int getClassAddress(char* name)//returns the index of Class 
{
    int i;
    for(i=1;i<numClasses;i++){
        if(!strcmp(name,classesST[i].className))
            return i;
    }
    return 0;
}
int getMethodAddress(char* name,int classId){//returns the index of method 
    int i;
    for( i=0;i<classesST[classId].numMethods;i++){
        if(!strcmp(classesST[classId].methodList[i].methodName,name))
            return i;
    }
    return -1;
}
void codeGenExpression(ASTree *exp,FILE *fp,int classContainingExprs,int methodContainingExprs){
    switch(exp->typ){
        case DOT_ID_EXPR:
            //need to implement

            return;
        case METHOD_CALL_EXPR:
            //need to implement
            return;
        case DOT_ASSIGN_EXPR:
            //need to implement
            return;
        case DOT_METHOD_CALL_EXPR:
            //need to implement
            return;
        case THIS_EXPR:
            //need to implement

            return;
        case NEW_EXPR:
            return;
        case ID_EXPR:
            if(classContainingExprs<0){
                int value = getMainIDNumber(exp->children->data->idVal);
                fprintf(fp, "mov 1 %d\t;loading id address into R[1]\n", MAX - value);
                fprintf(fp, "lod 1 1 0\t;storing ID value into R[1]\n");
                fprintf(fp, "str 6 0 1 \t;Storing ID value into stack and end of ID_EXPR\n");
                decSp(fp);
                return;
            }
            return;
        case ASSIGN_EXPR://find the address of lhs(same as ID) and compute rhs and store it in that address
            if(classContainingExprs<0){
                codeGenExpression(exp->children->next->data,fp,classContainingExprs,methodContainingExprs);//computing RHS first
                int value = getMainIDNumber(exp->children->data->idVal);
                fprintf(fp, "mov 1 %d\t;R[1]=address(LHS)\n", MAX - value);
                fprintf(fp, "lod 2 6 1\t;loading the value of RHS into R[2]\n");
                fprintf(fp, "str 1 0 2\t;Storing the address into memory END of the '=' EXP\n");
                return;
            }
            return;
        
        case AND_EXPR://E1&E2
            fprintf(fp,";AND EXPR starts here\n");
            int not=labelNumber++;
            int andEnd = labelNumber++;
            codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);//computing E1
            fprintf(fp, "lod 1 6 1\t;Loading e1 into R[1]\n");
            fprintf(fp, "beq 1 0 #ANDFalse%d\t;not-cond\n", not);
            codeGenExpression(exp->children->next->data,fp,classContainingExprs,methodContainingExprs);//computing E2
            fprintf(fp, "lod 1 6 1\t;Loading e1 into R[2]\n");
            fprintf(fp, "beq 1 0 #ANDFalse%d\t;not-cond\n", not);
            fprintf(fp,"mov 1 1\t;R[1]=1\n");
            fprintf(fp,"str 6 1 1\t;result=1\n");
            fprintf(fp,"jmp 0 #ANDEnd%d;\tjumps to and end\n",andEnd);
            fprintf(fp,"#ANDFalse%d:mov 0 0;\tand flase starts here\n",not);
            fprintf(fp,"str 6 1 0\t;result=0\n");
            fprintf(fp,"jmp 0 #ANDEnd:%d;\tjumps to and end\n",andEnd);
            fprintf(fp,"#ANDEnd%d: mov 0 0\n");
            decSp(fp);
            return;
        case EXPR_LIST:
            codeGenExpressions(exp->children,fp,classContainingExprs,methodContainingExprs);//computes each of the expresssion by invoking the method
            return;
        case NULL_EXPR:
            fprintf(fp,";Null expression computing\n");
            fprintf(fp,"str 6 0 0\t;storing 0 value in current stack pointer\n");
            decSp(fp);
            return;
        case PRINT_EXPR:
            fprintf(fp,";Print expression computing\n");
            codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);//computing the value of the expression
            fprintf(fp,"lod 1 6 1\t;loading the value exp into R[1]\n");//loads the result into the R[1]
            fprintf(fp,"ptn 1\t;prints the value in R[1]\n");//prints the result
            return;
        case READ_EXPR:
            fprintf(fp,";Read expression computing\n");
            fprintf(fp,"rdn 1\t;reading and storing the value in R[1]\n");//reading the value
            fprintf(fp,"str 6 0 1\t;storing the result in the stack\n");//storing the result in the stack
            decSp(fp);
            return;
        
        case GREATER_THAN_EXPR:
            codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);
            codeGenExpression(exp->children->next->data,fp,classContainingExprs,methodContainingExprs);

            int gLabel,trueLabel1;
            trueLabel1 = labelNumber++;
            gLabel=labelNumber++;
            fprintf(fp, "lod 1 6 2\t;e1 value\n");
            fprintf(fp, "lod 2 6 1\t;e2 value\n");
            fprintf(fp, "bgt 1 2 #GreaterLabel%d\t; > check\n", trueLabel1);
            fprintf(fp, "str 6 1 0\t;stores-zero\n");
            fprintf(fp, "jmp 0 #GreaterEnd%d\t;goes to end\n", gLabel);
            fprintf(fp, "#GreaterLabel%d: mov 1 1\t;true condition 1\n", trueLabel1);
            fprintf(fp, "str 6 1 1\t;stores-1\n");
            fprintf(fp, "#GreaterEnd%d:mov 0 0\t;end == \n", gLabel);
            decSp(fp);
            return;
            
        case EQUALITY_EXPR:
            codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);
            codeGenExpression(exp->children->next->data,fp,classContainingExprs,methodContainingExprs);
            int eLabel,trueLabel;
            trueLabel = labelNumber++;
            eLabel=labelNumber++;
            fprintf(fp, "lod 1 6 2\t;e1 value\n");
            fprintf(fp, "lod 2 6 1\t;e2 value\n");
            
            fprintf(fp, "beq 1 2 #EqualLabel%d\t; == check\n", trueLabel);
            fprintf(fp, "str 6 1 0\t;stores-zero\n");
            
            fprintf(fp, "jmp 0 #EqualEnd%d\t;goes to end\n", eLabel);
            fprintf(fp, "#EqualLabel%d: mov 1 1\t;true condition 1\n", trueLabel);
            fprintf(fp, "str 6 1 1\t;stores-1\n");
            
            fprintf(fp, "#EqualEnd%d:mov 0 0\t;end == \n", eLabel);
            decSp(fp);
           // incSp(fp); 
            return;
            //incSp(fp); 
        case NAT_LITERAL_EXPR:
            fprintf(fp,";Nat expression computing\n");
            fprintf(fp,"mov 1 %d\t;storing the nat value into the R[1]\n",exp->natVal);
            fprintf(fp,"str 6 0 1\t;storing the nat value into R[6] from R[1]\n");
            decSp(fp);
            return;
        case PLUS_EXPR://e1+e2
            
            codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);//computing e1
            codeGenExpression(exp->childrenTail->data,fp,classContainingExprs,methodContainingExprs);//computing e2
            fprintf(fp,";Plus expression computing\n");
            fprintf(fp,"lod 1 6 2\t;loading the result of e1 into R[1]\n");//e1->R[1]
            fprintf(fp,"lod 2 6 1\t;loading the result of e2 into R[2]\n");//e2->R[2]
            // fprintf(fp,"ptn 1\n");
            // fprintf(fp,"ptn 2\n");
            fprintf(fp,"add 1 1 2\t;R[1]=R[1](e1)+R[2](e2)\n");
            // fprintf(fp,"ptn 1\n");
            
            fprintf(fp,"str 6 2 1\t;storing the result into the stack\n");
            incSp(fp);
            return;
        case MINUS_EXPR://e1-e2 
        //similar to PLUS_EXPR
           
            codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);//computing e1
            codeGenExpression(exp->children->next->data,fp,classContainingExprs,methodContainingExprs);//computing e2
            fprintf(fp,";Minus expression computing\n");
            fprintf(fp,"lod 1 6 2\t;loading the result of e1 into R[1]\n");//e1->R[1]
            fprintf(fp,"lod 2 6 1\t;loading the result of e2 into R[2]\n");//e2->R[2]
            fprintf(fp,"sub 1 1 2\t;R[1]=R[1](e1)-R[2](e2)\n");
            fprintf(fp,"str 6 2 1\t;storing the result into the stack\n");
            incSp(fp);
            return;
        case TIMES_EXPR://e1*e2
        //similar to PLUS_EXPR
            fprintf(fp,";Times expression computing\n");
            codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);//computing e1
            codeGenExpression(exp->children->next->data,fp,classContainingExprs,methodContainingExprs);//computing e2
            fprintf(fp,"lod 1 6 2\t;loading the result of e1 into R[1]\n");//e1->R[1]
            fprintf(fp,"lod 2 6 1\t;loading the result of e2 into R[2]\n");//e2->R[2]
            fprintf(fp,"mul 1 1 2\t;R[1]=R[1](e1)*R[2](e2)\n");
            fprintf(fp,"str 6 2 1\t;storing the result into the stack\n");
            incSp(fp);
            return;
        case NOT_EXPR:
            codeGenExpression(exp->children->data,fp, classContainingExprs, methodContainingExprs);
            fprintf(fp, "lod 1 6 1;NOT_EXPR starts here\n");
            int notLabel=labelNumber++;
            int notEnd=labelNumber++;
            fprintf(fp, "beq 1 0 #NOT%d;not-cond\n", notLabel);
            fprintf(fp, "str 6 1 0\t;Storing 0\n");
            fprintf(fp, "jmp 0 #NOTEND%d\t;not-jmp-end\n", notEnd);
            fprintf(fp, "#NOT%d: mov 1 1\t;not-set-r1=1\n", notLabel);
            fprintf(fp, "str 6 1 1\t;storing 1\n");
            fprintf(fp, "#NOTEND%d: mov 0 0\t;Not ends here\n", notEnd);
            return;
        case IF_THEN_ELSE_EXPR:;
            fprintf(fp,";If else expression computing\n");
            int elseLabel,endLabel ;//if(e0){e1}else{e2}
            elseLabel=labelNumber++;
            endLabel = labelNumber++;
            codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);
            fprintf(fp,"lod 1 6 1\t;load e0 result into r1\n");
            fprintf(fp,"beq 0 1 #elseLabel%d\t;checking if e0==0 or not\n",elseLabel);
            incSp(fp);
            codeGenExpression(exp->children->next->data,fp,classContainingExprs,methodContainingExprs);
            fprintf(fp,"jmp 0 #endLabel%d\t;if condition ends here\n",endLabel);
            fprintf(fp,"#elseLabel%d:mov 0 0;else starts here\n",elseLabel);
            incSp(fp);
            codeGenExpression(exp->children->next->next->data,fp,classContainingExprs,methodContainingExprs);
            fprintf(fp,"#endLabel%d:mov 0 0\t;end of if and else\n",endLabel);
            return;
        case WHILE_EXPR:
        fprintf(fp,";While expression computing\n");//while starts here
        int startLoop = labelNumber++;
        fprintf(fp,"#LoopStart%d: mov 0 0\t;loop starts here\n",startLoop);
        codeGenExpression(exp->children->data,fp,classContainingExprs,methodContainingExprs);//codegen(e1)
        fprintf(fp,"mov 1 1\t;loading value into R[1]\n");
        fprintf(fp, "lod 2 6 1\t;condition check\n");//loading the result
        int trueCondition=labelNumber++;
        fprintf(fp,"bgt 2 0 #TrueCondition%d\t;if condition satisfies PC jumps to TrueCondition\n",trueCondition);//checking the result
        int endLabel12= labelNumber++;
        fprintf(fp,"jmp 0 #LoopEnd%d\t;JUMPs to end of the loop\n",endLabel12);
        fprintf(fp,"#TrueCondition%d:mov 0 0\t;While expressions below\n",trueCondition);
        codeGenExpression(exp->children->next->data,fp,classContainingExprs,methodContainingExprs);
        fprintf(fp,"jmp 0 #LoopStart%d\t;JUMPs to loop start\n",startLoop);
        fprintf(fp,"#LoopEnd%d:mov 0 0\t;Loop ends here\n",endLabel12);
        incSp(fp);
        return;
        default:
            //return;
            fprintf(fp,";unknown\n");
            return;


    }
}
void codeGenExpressions(ASTList *exp,FILE *fp,int classContainingExprs,int methodContainingExprs){
    //ASTList *expList = exp->children;
    //int countExps = countExpressions(expList);//stores number of expressions in the exp expressionList
    
    while(exp){//iterates=#number of expressions
        
        codeGenExpression(exp->data, fp ,classContainingExprs, methodContainingExprs);//generates code for a given expression
        incSp(fp);//increments stack pointer to reuse the memory
        exp = exp->next;//next expression
    }
}
void generateDISM(FILE *outputFile){
    initializeRegisters(outputFile);
    //loading main method variables into stack;
    int mainVariables=numMainBlockLocals;
   // fprintf(outputFile,";%d\n",mainVariables);
    while(mainVariables--){//itrations=#numMainBlockLocals
        fprintf(outputFile,"str 6 0 0\t;declaring main variable to zero\n");
        decSp(outputFile);
    }
    //executing main expression:
    codeGenExpressions(mainExprs->children,outputFile, -1, -1);//code generation for main Expression similar to typecheck.c
    generateVtable(outputFile);
    genPrologue(outputFile);
    genEpilogue(outputFile);
    fprintf(outputFile,"hlt 0;end of the program");
}


