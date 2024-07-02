const path = require('path');
const addon = require('../build/Release/primary-color');
console.log(addon);



const fs = require('fs');

let result = addon.primaryColorByImageUrl(
    [
        "https://cdn.discordapp.com/banners/774409449476980746/f67cd20df77e4c15de55a5da0c92bc4c.png?size=1024&format=webp&quality=lossless&width=0&height=256",
        "344",
        "@3232",
        "https://i.pinimg.com/736x/30/0c/30/300c3027c4eec274de2a15bacfc7755f.jpg", "https://i.pinimg.com/736x/dd/11/77/dd1177ae42e5ffcc5572ab9d861b1ef2.jpg"
    ],
    'hex'
);

console.log(result);

let base64_img = get_image_from_fs("images.jpeg");
let base64_img1 = get_image_from_fs("c1ba9a1b65ba3e3f3cf0ca43cab00b1d.jpg")

let result1 = addon.primaryColorByBase64([base64_img, base64_img1]);

console.log(result1);

function get_image_from_fs(image_string) {
    try {

        let image = fs.readFileSync('./test/' + image_string, 'base64');

        return image;

    } catch (e) {
        console.error(e);
    }
}