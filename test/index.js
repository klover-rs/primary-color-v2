const addon = require('./build/Release/primary-color.node');

console.log(addon.hello());

let result = addon.primaryColor("https://cdn.discordapp.com/banners/774409449476980746/f67cd20df77e4c15de55a5da0c92bc4c.png?size=1024&format=webp&quality=lossless&width=0&height=256");

console.log(result);