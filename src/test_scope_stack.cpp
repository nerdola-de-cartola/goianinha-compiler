#include <iostream>
#include "scope_stack.hpp"

void test_get_var(ScopeStack &scope_stack, std::string name) {
    Variable *var = scope_stack.get_variable(name);

    if (var == nullptr) {
        std::cout << "Variable " << name << " not found.\n";
        return;
    }

    std::cout << var->toString() << " was found.\n";
}

void test_get_function(ScopeStack &scope_stack, std::string name) {
    Function *f = scope_stack.get_function(name);

    if (f == nullptr) {
        std::cout << "Function " << name << " not found.\n";
        return;
    }

    std::cout << f->toString() << " was found.\n";
}

void full_test(ScopeStack &scope_stack) {
    std::cout << "\n" << scope_stack.toString() << "\n";

    test_get_var(scope_stack, "i");
    test_get_var(scope_stack, "a");
    test_get_var(scope_stack, "_x");
    test_get_var(scope_stack, "_b12");

    std::cout << "\n";

    test_get_function(scope_stack, "funcao_n_existe");
    test_get_function(scope_stack, "funcao_basica");
    test_get_function(scope_stack, "funcao_exemplo");
    test_get_function(scope_stack, "funcao_com_varios_parametros");
}

int main() {
    ScopeStack* scope_stack = new ScopeStack();
    
    Function f1 = Function("funcao_basica", CAR);
    Function f2 = Function("funcao_exemplo", CAR);
    f2.add_parameter(Variable("x", INT));
    Function f3 = Function("funcao_com_varios_parametros", INT);
    f3.add_parameter(Variable("x", INT));
    f3.add_parameter(Variable("y", CAR));
    f3.add_parameter(Variable("z", INT));
    f3.add_parameter(Variable("w", CAR));
    
    scope_stack->push();
    scope_stack->add_variable(Variable("a", INT));
    scope_stack->add_variable(Variable("_b12", CAR));
    scope_stack->add_function(f1);
    
    scope_stack->push();
    scope_stack->add_variable(Variable("a", INT));
    scope_stack->add_variable(Variable("_x", CAR));
    scope_stack->add_function(f1);
    scope_stack->add_function(f2);
    scope_stack->add_function(f3);
    
    std::cout << "Testando a pilha de escopos...\n";
    std::cout << "Buscando na pilha...\n";
    full_test(*scope_stack);
    std::cout << "\nAgora vamos apagar o escopo do topo e fazer a busca novamente\n";
    scope_stack->pop();
    full_test(*scope_stack);

    delete scope_stack;
    
    std::cout << "\nFim do teste." << '\n';
    return 0;
}
