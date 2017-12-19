const computeFibonacci = require('./build/Release/addon');

function print(number) {
  console.log(number);
}

computeFibonacci(10, print);
