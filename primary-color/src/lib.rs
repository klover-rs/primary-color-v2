
use std::fmt::format;
use std::sync::{Arc, Mutex};
use std::{ffi::CString, os::raw::c_char};
use std::ptr;
use std::ffi::CStr;
use convert::convert_to_vec;
use rgb::{Color, HexOrRgb};
use validate::{check_if_is_array, validate_base64, validate_url};
use serde_json::json;

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

        let valid_results = Arc::new(Mutex::new(Vec::new()));
        let (sender, receiver) = crossbeam_channel::unbounded();

        crossbeam::scope(|scope| {
            for url in url_array {
                let sender = sender.clone();
                let valid_results = Arc::clone(&valid_results); 
                scope.spawn(move |_| {
                    match validate_url(&url) {
                        Ok(b) => {
                            if b {
                                match image::image_primary_color_by_url(&url, hex_or_rgb) {
                                    Ok(s) => {
                                        let result = match s {
                                            Color::Hex(hex) => format!("[{}]", hex),
                                            Color::Rgb(r, g, b) => {
                                                let rgb_json = json!({
                                                    "r": r,
                                                    "g": g,
                                                    "b": b
                                                });
                                                format!("[{}]", rgb_json.to_string())
                                            }
                                        };

                                        let mut results = valid_results.lock().unwrap();
                                        results.push(result);
                                        let _ = sender.send(());
                                    }
                                    Err(_) => {}
                                }
                            }
                        }
                        Err(_) => {}
                    }
                });
              
            }
        }).unwrap();

     
        drop(sender);

        for _ in receiver.iter() {}

        let results = valid_results.lock().unwrap();
        if results.is_empty() {
            eprintln!("no valid results could be found");
            return ptr::null();
        }

        let result_string = results.join(", ");
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
                            let color = match s {
                                Color::Hex(hex) => {
                                    hex
                                }
                                Color::Rgb(r, g, b) => {

                                    let rgb_json = json!({
                                        "r": r,
                                        "g": g,
                                        "b": b
                                    });

                                    rgb_json.to_string()
                                }
                            };

                            let color = CString::new(color).unwrap();
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

    let is_array = check_if_is_array(&base64);
    if is_array {
        let base64_array = convert_to_vec(base64);

        let valid_results = Arc::new(Mutex::new(Vec::new()));
        let (sender, receiver) = crossbeam_channel::unbounded();

        crossbeam::scope(|scope| {
            for base64 in base64_array {
                let sender = sender.clone();
                let valid_results = Arc::clone(&valid_results);

                scope.spawn(move |_| {
                    match validate_base64(&base64) {
                        Ok(b) => {
                            if b {
                                match image::image_primary_color_by_base64(&base64, hex_or_rgb) {
                                    Ok(s) => {
                                        let result = match s {
                                            Color::Hex(hex) => format!("[{}]", hex),
                                            Color::Rgb(r, g, b) => {
        
                                                let rgb_json = json!({
                                                    "r": r,
                                                    "g": g,
                                                    "b": b
                                                });
                                                format!("[{}]", rgb_json.to_string())
                                            }
                                        };

                                        let mut results = valid_results.lock().unwrap();
                                        results.push(result);
                                        let _ = sender.send(());
                                    },
                                    Err(_) => {}
                                }
                            }
                        }
                        Err(_) => {}
                    }
                });
            }
        }).unwrap();

        drop(sender);

        for _ in receiver.iter() {}

        let results = valid_results.lock().unwrap();
        if results.is_empty() {
            eprintln!("no valid results could be found");
            return ptr::null();
        }

        let result_string = results.join(", ");
        let mut result = String::new();
        result.push_str("[");
        result.push_str(&result_string);
        result.push_str("]");

        let colors = CString::new(result).unwrap();
        return colors.into_raw();
    } else {
        match validate_base64(&base64) {
            Ok(b) => {
                if b {
                    match image::image_primary_color_by_base64(&base64, hex_or_rgb) {
                        Ok(s) => {
                            let color = match s {
                                Color::Hex(hex) => {
                                    hex
                                }
                                Color::Rgb(r, g, b) => {

                                    let rgb_json = json!({
                                        "r": r,
                                        "g": g,
                                        "b": b
                                    });

                                    rgb_json.to_string()
                                }
                            };

                            let color = CString::new(color).unwrap();
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
}