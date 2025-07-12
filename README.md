# Calculator

It's a simple terminal based calculator which utilixes the addition, subtraction, multiplication and division. It has also support for unary operations such as '+' and '-'. It works by parsing numbers, unary and binary operations. Each one have a seperate function and works on the fact that after number there is binary operation required and after unary operation there can be either a number or another unary operation.

Building the expression/binary tree can be intuitively imagined as linked lists with two ends. Once the condition is met, the whole tree to the right is pushed down and a new branch is thus formed. That's how I imagined and was able to form function to insert nodes.

# Compilation
1. Clone the repository by typing the command `https://github.com/keyboardNinjaWarrior/calculator.git` in terminal.
2. Go into the repository
3. Enter the command `make`
4. Then type `./calc <expression>`
