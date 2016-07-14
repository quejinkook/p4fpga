/*
Copyright 2013-present Barefoot Networks, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _BACKENDS_FPGA_CODEGEN_H_
#define _BACKENDS_FPGA_CODEGEN_H_

#include "ir/ir.h"
#include "lib/sourceCodeBuilder.h"
//#include "target.h"
#include "frontends/p4/typeMap.h"

namespace FPGA {

class CodeBuilder : public Util::SourceCodeBuilder {
 public:
    //const Target* target;
    //explicit CodeBuilder(const Target* target) : target(target) {}
};

// Visitor for generating C for FPGA
// This visitor is invoked on various subtrees
class CodeGenInspector : public Inspector {
 protected:
    CodeBuilder*       builder;
    const P4::TypeMap* typeMap;

 public:
    CodeGenInspector(CodeBuilder* builder, const P4::TypeMap* typeMap) :
            builder(builder), typeMap(typeMap) { visitDagOnce = false; }

    using Inspector::preorder;

    bool preorder(const IR::Constant* expression) override;
    bool preorder(const IR::Declaration_Variable* decl) override;
    bool preorder(const IR::BoolLiteral* b) override;
    bool preorder(const IR::Cast* c) override;
    bool preorder(const IR::Operation_Binary* b) override;
    bool preorder(const IR::Operation_Unary* u) override;
    bool preorder(const IR::ArrayIndex* a) override;
    bool preorder(const IR::Mux* a) override;
    bool preorder(const IR::Member* e) override;
    bool comparison(const IR::Operation_Relation* comp);
    bool preorder(const IR::Equ* e) override { return comparison(e); }
    bool preorder(const IR::Neq* e) override { return comparison(e); }

    bool preorder(const IR::IndexedVector<IR::StatOrDecl>* v) override;
    bool preorder(const IR::Path* path) override;

    bool preorder(const IR::Type_Typedef* type) override;
    bool preorder(const IR::Type_Enum* type) override;
    bool preorder(const IR::AssignmentStatement* s) override;
    bool preorder(const IR::BlockStatement* s) override;
    bool preorder(const IR::MethodCallStatement* s) override;
    bool preorder(const IR::EmptyStatement* s) override;
    bool preorder(const IR::ReturnStatement* s) override;
    bool preorder(const IR::ExitStatement* s) override;
    bool preorder(const IR::IfStatement* s) override;

    void widthCheck(const IR::Node* node) const;

 protected:
    struct VecPrint {
        cstring separator;
        cstring terminator;

        VecPrint(const char* sep, const char* term) :
                separator(sep), terminator(term) {}
    };

    // maintained as stack
    std::vector<VecPrint> vectorSeparator;

    void setVecSep(const char* sep, const char* term = nullptr) {
        vectorSeparator.push_back(VecPrint(sep, term));
    }
    void doneVec() {
        BUG_CHECK(!vectorSeparator.empty(), "Empty vectorSeparator");
        vectorSeparator.pop_back();
    }
    VecPrint getSep() {
        BUG_CHECK(!vectorSeparator.empty(), "Empty vectorSeparator");
        return vectorSeparator.back();
    }
};

}  // namespace FPGA


#endif /* _BACKENDS_FPGA_CODEGEN_H_ */