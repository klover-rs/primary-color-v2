# primary-color-v2
This project is simply just used for getting the primary color of an image in nodejs. Please note this is a **Native Node Module**

## How to build your Node Module

### prerequisites
- C++ build tools for your platform (you have to anyways install them while you install rust or nodejs)
- [nodejs](https://nodejs.org/en/download/package-manager)
- [rust](https://www.rust-lang.org/tools/install) (only required for manual building)
- [python](https://www.python.org/downloads/)
- node-gyp (can be installed with `npm i -g node-gyp`)

we have to methods for building, a python script (coming soon), or manual building

### Method 1. installer script

just run `npm i` in the root dir of this project

### Method 2. Manual Building (assumes linux or macOS)

for this method we need some additional prerequisites which are optional tho.

- cbindgen (not required for manual building but can be useful);

this tutorial expects that you use a shell

1. Clone the repo
2. open up ur terminal
3. in the root directory of the project: `cd primary-color && cargo build --release`

please note that the file name can be different on different OSes, on macos for example "libprimary_image_color.a" or on windows "libprimary_image_color.lib"
4. `cd .. && mkdir lib && mv primary-color/target/release/libprimary_image_color.a lib`
5. `node-gyp configure && node-gyp build`

### Note for windows

once you have completed the building, you need to move the "primary_image_color.dll" file from the `primary-color/target/release` folder into the `build/Release` folder


## Test it out!

you can test it out with `npm test` in the root directory (make sure that you have built everything tho :D)

## how does it even work? 

well well well
This project uses multiple technologies 
those are 

- Rust 
- C++ 

you can imagine it like a vpn 

C++ is used to get the inputs from javascript and return values to javascript, I also decided to use C++ for converting certain stuff etc etc. 

Rust on the other hand is where the core, aka the heart of this project lays. I decided to use Rust for the processing of images, and calculating the primary color. 

This project uses various techniques to make the usage as simple as possible. for example: 

if you want to process multiple images, no problem, just create an array in javascript like this 

```js
    let result = addon.primaryColorByImageUrl(
    [
        "https://cdn.discordapp.com/banners/774409449476980746/f67cd20df77e4c15de55a5da0c92bc4c.png?size=1024&format=webp&quality=lossless&width=0&height=256",
        "344",
        "@3232",
        "https://i.pinimg.com/736x/30/0c/30/300c3027c4eec274de2a15bacfc7755f.jpg", "https://i.pinimg.com/736x/dd/11/77/dd1177ae42e5ffcc5572ab9d861b1ef2.jpg"
    ]);
```

more examples can be found in `test/index.js`