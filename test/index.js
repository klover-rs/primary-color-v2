const path = require('path');
const addon = require('../dist/index');
console.log(addon);



const fs = require('fs');

let result = addon.primaryImageColorFromUrl(
    [
        "https://cdn.discordapp.com/banners/774409449476980746/f67cd20df77e4c15de55a5da0c92bc4c.png?size=1024&format=webp&quality=lossless&width=0&height=256", // a link which doesnt work anymore
        "344", 
        "@3232", // invalid urls right here
        "https://i.pinimg.com/736x/30/0c/30/300c3027c4eec274de2a15bacfc7755f.jpg", "https://i.pinimg.com/736x/dd/11/77/dd1177ae42e5ffcc5572ab9d861b1ef2.jpg" 
    ],
    'hex'
);

let result_non_array = addon.primaryImageColorFromUrl("https://i.pinimg.com/736x/30/0c/30/300c3027c4eec274de2a15bacfc7755f.jpg", "rgb");

console.log(result);

console.log(result_non_array);

let base64_img = get_image_from_fs("images.jpeg");
let base64_img1 = get_image_from_fs("c1ba9a1b65ba3e3f3cf0ca43cab00b1d.jpg")

let result1 = addon.primaryImageColorFromBase64([base64_img, base64_img1], "rgb");
let result1_non_array = addon.primaryImageColorFromBase64(base64_img, "hex");

console.log(result1_non_array);

console.log(result1);

function get_image_from_fs(image_string) {
    try {

        let image = fs.readFileSync('./test/' + image_string, 'base64');

        return image;

    } catch (e) {
        console.error(e);
    }

}
