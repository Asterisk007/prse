extern crate pest;

use crate::parser::AstNode::Print;
use pest::{Parser, error::Error};
#[derive(Parser)]
#[grammar = "grammar.pest"]
pub struct PParser;

#[derive(Debug)]
pub enum MathOperator {
    Plus,
    Minus,
    Multiply,
    Divide,
    Equal,
    LessThan,
    GreaterThan,
}

#[derive(Debug)]
pub enum AstNode {
    Print(Box<AstNode>),
    Integer(i32),
    Float(f64),
    Str(String),
    MathExpr {
        op: MathOperator,
        lhs: Box<AstNode>,
        rhs: Box<AstNode>
    },
    Identifier(String)
}

pub fn parse(source: &str) -> Result<Vec<AstNode>, Error<Rule>> {
    let mut ast = vec![];

    let pairs = PParser::parse(Rule::valid_input, source)?;
    for pair in pairs {
        println!("{:?}", pair);
        match pair.as_rule() {
            Rule::expr => {
                ast.push(Print(Box::new(build_ast_from_expr(pair))));
            },
            _ => {}
        }
    }

    Ok(ast)
}


pub fn build_ast_from_expr(pair: pest::iterators::Pair<Rule>) -> AstNode {
    match pair.as_rule() {
        Rule::math_expr => {
            let mut pair = pair.into_inner();
            let lhspair = pair.next().unwrap();
            let lhs = build_ast_from_expr(lhspair);
            let operator = pair.next().unwrap();
            let rhspair = pair.next().unwrap();
            let rhs = build_ast_from_expr(rhspair);
            parse_binary_op(operator, lhs, rhs)
        }
        unknown => panic!("Unexpected expression {:?}", unknown),
    }
}

pub fn parse_binary_op(pair: pest::iterators::Pair<Rule>, lhs: AstNode, rhs: AstNode) -> AstNode {
    AstNode::MathExpr {
        lhs: Box::new(lhs),
        rhs: Box::new(rhs),
        op: match pair.as_str() {
            "+" => MathOperator::Plus,
            "-" => MathOperator::Minus,
            "/" => MathOperator::Divide,
            "*" => MathOperator::Multiply,
            _ => panic!("Unexpected math operator: {}", pair.as_str())
        }
    }
}

#[cfg(test)]
mod tests {
    use pest::Parser;
    #[derive(Parser)]
    #[grammar = "grammar.pest"]
    pub struct PParser;

    #[test]
    fn test_number_parsing() {
        assert!(PParser::parse(Rule::number, "123").is_ok());
    }

    #[test]
    fn test_identifier_parsing() {
        assert!(PParser::parse(Rule::identifier, "abc").is_ok());
    }

    #[test]
    fn test_identifier_with_underscores() {
        assert!(PParser::parse(Rule::identifier, "__abc_with_underscores__").is_ok());
    }

    #[test]
    fn test_assignment_parsing() {
        assert!(PParser::parse(Rule::valid_input, "abc = 123").is_ok());
    }
}