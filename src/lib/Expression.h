#pragma once

// The generic Expression class which all expressions are made of
class Expression {
public:
   virtual void execute() {return nullptr};
   virtual void type() {return nullptr};
};
