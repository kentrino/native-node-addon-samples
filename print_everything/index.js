const addon = require('./build/Release/addon');
addon.printEverything(1, "a", [1,2,3], {
  a: "alpha",
  b: "beta",
});
