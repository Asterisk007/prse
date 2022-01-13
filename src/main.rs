mod parser;
use parser::parse;

#[macro_use]
extern crate pest_derive;

fn main() {
    let file = std::fs::read_to_string("example.prse").expect("Cannot read file");
    let ast_node = parse(&file).expect("unsuccessful parse");
    println!("{:?}", &ast_node);
}