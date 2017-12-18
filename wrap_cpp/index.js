const SimpleObject = require('./build/Release/addon');
const s = new SimpleObject();
// TODO: プロトタイプ拡張はできない

console.log(s.hello);

for (let i = 0; i < 10000000; ++i) {
  s.push(i);
}
for (let i = 0; i < 10000000; ++i) {
  s.pop();
}
