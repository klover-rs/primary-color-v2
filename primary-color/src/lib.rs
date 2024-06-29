
use std::{ffi::CString, os::raw::c_char};
use std::ptr;
use std::ffi::CStr;
use convert::convert_to_vec;
use rgb::HexOrRgb;
use validate::{check_if_is_array, validate_base64, validate_url};

mod validate;
mod rgb;
mod image;
mod convert;

#[no_mangle]
pub extern "C" fn test_fn(hex_or_rgb: HexOrRgb) {
    match hex_or_rgb {
        HexOrRgb::Hex => {
            println!("will return a hex code");
        }
        HexOrRgb::Rgb => {
            println!("will return RGB values");
        }
    }
}

#[no_mangle]
pub extern "C" fn primary_color_from_image_url(image_url: *const c_char, hex_or_rgb: HexOrRgb) -> *const c_char {

    if image_url.is_null() {
        println!("Error: image_url pointer is null");
        return ptr::null();
    }

    let c_str = unsafe {
        CStr::from_ptr(image_url)
    };

    let url = match c_str.to_str() {
        Ok(s) => s,
        Err(e) => {
            eprintln!("failed to convert C String to UTF8: {}", e);
            return ptr::null();
        }
    };

    let is_array = check_if_is_array(&url);
    if is_array {
        let url_array = convert_to_vec(&url);

        let mut valid_results = Vec::new();
        let mut index = 0;

        for url in url_array {
            match validate_url(&url) {
                Ok(b) => {
                    if b {
                        match image::image_primary_color_by_url(&url, hex_or_rgb) {
                            Ok(s) => {
                                valid_results.push(s);
                            },
                            Err(_) => {
                                continue;
                            }
                        }
                    }
                }
                Err(_) => {
                    continue;
                }
            }

            index += 1;
        }
        
        if valid_results.is_empty() {
            eprintln!("no valid results could be found");
            return ptr::null();
        }

        let result_string = valid_results.join(", ");
        let mut result = String::new();
        result.push_str("[");
        result.push_str(&result_string); 


        result.push_str("]");

        let colors = CString::new(result).unwrap();
        return colors.into_raw();
        

    } else {
        match validate_url(&url) {
            Ok(b) => {
                if b {
                    match image::image_primary_color_by_url(&url, hex_or_rgb) {
                        Ok(s) => {
                            let color = CString::new(s).unwrap();
                            return color.into_raw();
                        },
                        Err(e) => {
                            eprintln!("error: {}", e);
                            return ptr::null();
                        }
                    };
                    
                } else {
                    eprintln!("not a valid url.");
                    return ptr::null();
                }
            }
            Err(e) => {
                eprintln!("error: {}", e);
                return ptr::null();
            }
        }
    }

    
}

#[no_mangle]
pub extern "C" fn primary_color_from_base64(base64: *const c_char, hex_or_rgb: HexOrRgb) -> *const c_char {
    if base64.is_null() {
        println!("Error: base64 pointer is null");
        return ptr::null();
    }

    let c_str = unsafe {
        CStr::from_ptr(base64)
    };

    let base64 = match c_str.to_str() {
        Ok(s) => s,
        Err(e) => {
            eprintln!("failed to convert C String to UTF8: {}", e);
            return ptr::null();
        }
    };

    match validate_base64(&base64) {
        Ok(b) => {
            if b {
                match image::image_primary_color_by_base64(&base64, hex_or_rgb) {
                    Ok(s) => {
                        let color = CString::new(s).unwrap();
                        return color.into_raw();
                    },
                    Err(e) => {
                        eprintln!("error: {}", e);
                        return ptr::null();
                    }
                }
            } else {
                eprintln!("not a valid base64");
                return ptr::null();
            }
        }
        Err(e) => {
            eprintln!("Error: {}", e);
            return ptr::null();
        }
    }
}