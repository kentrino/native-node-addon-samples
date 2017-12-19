const SimpleObject = require('./build/Release/addon');
const s = new SimpleObject();

console.log(s.hello); // world
console.log(s.hasOwnProperty("hello")); // true

for (let i = 0; i < 10000000; ++i) {
  s.push(i);
}
for (let i = 0; i < 10000000; ++i) {
  s.pop();
}

// TODO: プロトタイプ拡張はできない
