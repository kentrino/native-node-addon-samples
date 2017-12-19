const computeFibonacci = require('./build/Release/addon');

function print(number) {
  console.log(number);
}

computeFibonacci(1000, print);
