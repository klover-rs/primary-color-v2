
use std::{ffi::CString, os::raw::c_char};
use std::ptr;
use std::ffi::CStr;
use validate::{validate_base64, validate_url};

mod validate;
mod rgb;
mod image;

#[no_mangle]
pub extern "C" fn primary_color() {
    println!("hello from rust!");
}

#[no_mangle]
pub extern "C" fn primary_color_from_image_url(image_url: *const c_char) -> *const c_char {

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

    match validate_url(&url) {
        Ok(b) => {
            if b {
                match image::image_primary_color_by_url(&url) {
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