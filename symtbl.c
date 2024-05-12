//I pledge my Honor that I have not cheated, and will not cheat, on this assignment.--SUDHAMSH REDDY DOKURU
#include <symtbl.h>
#include <stdio.h>
#include <string.h>

int typeNameToNumber(char *typeName){
    if(!strcmp("nat",typeName))
    {
        return -1;
    }
    if(typeName==NULL)
    return 0;//every class extends Object class
    for(int i=0;i<numClasses;i++){
        if(!strcmp(classesST[i].className,typeName))
            return i;//if 0=>Object else [1,2,3,..] return user defined class numbers
    }
    return -7;//undefined
}
int typeLineNumberfromNumber(int typ){
    if(typ==-1){
        return -1;
    }
    for(int i=0;i<numClasses;i++){
        if(!strcmp(classesST[i].className,typeName))
            return classesST[i].classNameLineNumber;//returns the line number of given type
    }
    return -7;//undefined case
}
void countNumberOfClasses(ASTree *t){//returns number of classes in given program
    numClasses=1;//initialize with 1 because of Object class
    //pgm->children==>(returns)list(classList,mainVairableList.mainMethodList)
    ASTList *x = t->children->data->children;//(returns first child i.e first class)
    while(x!=NULL&&x->data!=NULL){
        numClasses++;
        x=x->next;
    }
}

int countNumberOfMethodsAndVariables(ASTree *t){//returns number of mathods or variables based on the parameters
    int count=0;
    ASTList* temp = t->children;
    while(temp!=NULL&&temp->data!=NULL){
        count++;
        temp=temp->next;
    }
    return count;
}
void declareVariales(VarDecl* v,ASTree* vData,int numberOfVariables){//assigns appropreate type value to each variable in a method
    ASTList* temp = vData->children;
    int i=0;
    while(i<numberOfVariables){//no.iterates==> numberOfVariables times
        ASTree* variableData = temp->data;
        v[i].type =typeNameToNumber(variableData->children->data->idVal);//-7==>invalid,-1==>nat,0==>Object,[1,2,3...]==>userdefined
        v[i].typeLineNumber = typeLineNumberfromNumber(v[i].type);//line number of the type declaration
        v[i].varName = variableData->children->data->idVal;//name
        v[i].varNameLineNumber = variableData->children->data->lineNumber;
    } 
}
void declareMethodsList(MethodDecl* m,ASTree* classData,int numberOfMethods){//assigns data into the methodDecl list
    ASTList* temp = classData->children;
    int i=0;
    while(numberOfMethods>i){//iterates numberOfMethods times
        ASTree* methodData = temp->data;
        m[i].methodName = methodData.idVal;//name of the method
        m[i].returnType=methodData->children->data->idVal;//return type of the method
        m[i].methodNameLineNumber = methodData->lineNumber;//line no. of the method declaration
        m[i].returnTypeLineNumber = methodData->children->data->lineNumber;//line no. of the return type
        m[i].bodyExprs = methodData->childrenTail->data;//method body
        m[i].numParams = countNumberOfMethodsAndVariables(methodData->children->next->next->data);//returns number of parameters
        m[i].paramST = (VarDecl*)(malloc((m[i].numParams)*(sizeof(VarDecl))));//assigns memory=no.of parameters*(sizeof(VarDecl)
        declareVariales(m[i].paramST,methodData->children->next->next->data,m[i].numParams);//assigns appropreate type value to each parameters
        m[i].numLocals = countNumberOfMethodsAndVariables(methodData->children->next->next->next->data);//same as above but for method variables
        m[i].localST = (VarDecl*)(malloc((m[i].numLocals)*(sizeof(VarDecl))));
        declareVariales(m[i].localST,methodData->children->next->next->next->data,m[i].localST);
        i++;//inc i
    }
}
void setupSymbolTables(ASTree *fullProgramAST) {//root function
    countNumberOfClasses(fullProgramAST);//assigns numClasses to totla number of classess
    classesST=(ClassDecl*)malloc(numClasses*sizeof(ClassDecl));
    classesST[0].className="Object";//Object class declaration==>Object, if i = 0
    classesST[0].classNameLineNumber = -1;
    classesST[0].superclass = -3;//"no-object" (i.e., the type of Object's superclass), if i = -3
    classesST[0].numMethods=0;
    classesST[0].numVars=0;
    classesST[0].superclassLineNumber=-1;//object can't extend/have super class
    classesST[0].varList=NULL;
    classesST[0].methodList=NULL;//object class have no methods or variables
    //loop for numclasses-1 times to get the data related to remaining classes
    int i=1;
    ASTList * temp=fullProgramAST->children->data->children;//similar to countNumberOfClasses
    while(i<numClasses){//classNode's children order==>(AST_ID(name of class),AST_ID(superClass),VarDeclareList,MethodDeclareList)
        ASTree *currentClass = temp->data; 
        classesST[i].className=currentClass->idVal;//assigning class name
        classesST[i].classNameLineNumber = currentClass->lineNumber;//assigning Linenumn=ber
        classesST[i].numMethods = countNumberOfMethodsAndVariables(currentClass->children->next->next->next->data);//returns number of methods
        classesST[i].numVars = countNumberOfMethodsAndVariables(currentClass->children->next->next->data);//returns number of variables
        classesST[i].superclass= 0;//super class is assigned after this loop to avoid invalid errors
        classesST[i].methodList = (MethodDecl*)malloc(classesST[i].numMethods*sizeof(MethodDecl));//allocating space to method list size=no.methods*size of struct
        declareMethodsList(classesST[i].methodList,currentClass->children->next->next->data,classesST[i].numMethods);//aasign methods data into the list
        classesST[i].varList = (VarDecl*)malloc(classesST[i].numVars*sizeof(VarDecl));//same as above for the variables
        declareVariales(classesST[i].varList,,currentClass->children->next->next->data,classesST[i].numVars);
        temp=temp->next;//travelling to next node
        i++;
    }
    //this loop to assign superclass to given 
    i=1;
    temp=fullProgramAST->children->data->children;//similar to countNumberOfClasses
    while(i<numClasses){//classNode's children order==>(AST_ID(name of class),AST_ID(superClass),VarDeclareList,MethodDeclareList)
        ASTree *currentClass = temp->data; 
        classesST[i].superclass= typeNameToNumber(currentClass->children->next->data);//need to implement
        temp=temp->next;//travelling to next node
        i++;
    }
    //same operation done on methods is done to main method
    mainExprs = fullProgramAST->childrenTail->data;//main exps
    numMainBlockLocals = countNumberOfMethodsAndVariables(fullProgramAST->children->next->data);//no.of local Variables in main
    mainBlockST = (VarDecl*)malloc(sizeof(VarDecl)*numMainBlockLocals);
    declareVariales(mainBlockST,fullProgramAST->children->next->data,numMainBlockLocals);//same as above
    wholeProgram = fullProgramAST;//whole program ast

}