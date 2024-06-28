use anyhow::Result;

use regex::Regex;


pub fn validate_url(url: &str) -> Result<bool> {
    let url_pattern = r"^(http|https)://([a-zA-Z0-9\-_]+\.)+[a-zA-Z]{2,}(/[a-zA-Z0-9\-_./?%&=]*)?$";

    let rex = Regex::new(&url_pattern)?;

    Ok(rex.is_match(url))
}

pub fn validate_base64(base64: &str) -> Result<bool> {
    let base64_pattern = r"^(?:[A-Za-z0-9+/]{4})*(?:[A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=)?$";

    let rex = Regex::new(&base64_pattern).unwrap(); // :D 

    
    Ok(rex.is_match(base64))
    

}