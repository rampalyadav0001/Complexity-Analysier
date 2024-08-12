#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;

// Visitor class to traverse the AST and find loops
class LoopVisitor : public RecursiveASTVisitor<LoopVisitor> {
public:
    explicit LoopVisitor(ASTContext *Context) : Context(Context), NestingLevel(0) {}

    bool VisitForStmt(ForStmt *S) {
        if (isInMainFile(S)) {
            ++NestingLevel;
            llvm::outs() << "Found a for loop at line "
                         << Context->getSourceManager().getSpellingLineNumber(S->getForLoc())
                         << ", nesting level: " << NestingLevel << "\n";
            analyzeLoopComplexity(S);
            TraverseStmt(S->getBody()); // Traverse the body to detect nested loops
            --NestingLevel;
        }
        return true;
    }

    bool VisitWhileStmt(WhileStmt *S) {
        if (isInMainFile(S)) {
            ++NestingLevel;
            llvm::outs() << "Found a while loop at line "
                         << Context->getSourceManager().getSpellingLineNumber(S->getWhileLoc())
                         << ", nesting level: " << NestingLevel << "\n";
            analyzeLoopComplexity(S);
            TraverseStmt(S->getBody()); // Traverse the body to detect nested loops
            --NestingLevel;
        }
        return true;
    }

    bool VisitDoStmt(DoStmt *S) {
        if (isInMainFile(S)) {
            ++NestingLevel;
            llvm::outs() << "Found a do-while loop at line "
                         << Context->getSourceManager().getSpellingLineNumber(S->getDoLoc())
                         << ", nesting level: " << NestingLevel << "\n";
            analyzeLoopComplexity(S);
            TraverseStmt(S->getBody()); // Traverse the body to detect nested loops
            --NestingLevel;
        }
        return true;
    }

private:
    ASTContext *Context;
    int NestingLevel;

    bool isInMainFile(Stmt *S) {
        return Context->getSourceManager().isInMainFile(S->getBeginLoc());
    }

    void analyzeLoopComplexity(Stmt *S) {
        // Simplified complexity analysis: you can expand this as needed
        llvm::outs() << "Estimated time complexity: O(n^" << NestingLevel << ")\n";
    }
};

// Consumer class that uses the visitor
class LoopASTConsumer : public ASTConsumer {
public:
    explicit LoopASTConsumer(ASTContext *Context) : Visitor(Context) {}

    virtual void HandleTranslationUnit(ASTContext &Context) override {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    LoopVisitor Visitor;
};

// Frontend action class
class LoopFrontendAction : public ASTFrontendAction {
public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
        return std::make_unique<LoopASTConsumer>(&CI.getASTContext());
    }
};

// Main function to run the Clang Tool
int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, llvm::cl::GeneralCategory);
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

    return Tool.run(newFrontendActionFactory<LoopFrontendAction>().get());
}
