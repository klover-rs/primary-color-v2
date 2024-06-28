
use color_thief::ColorFormat;
use reqwest::blocking::Client;

use crate::rgb::get_hex_code_from_img;
use base64::{prelude::BASE64_STANDARD, Engine};

use anyhow::Result;

pub fn get_image_buffer(img: image::DynamicImage) -> (Vec<u8>, ColorFormat) {
    match img {
        image::DynamicImage::ImageRgb8(buffer) => {
            (buffer.to_vec(), color_thief::ColorFormat::Rgb)
        }
        image::DynamicImage::ImageRgba8(buffer) => {
            (buffer.to_vec(), color_thief::ColorFormat::Rgba)
        }
        _ => unreachable!(),
    }
}

pub fn image_primary_color_by_url(url: &str) -> Result<String> {
    let image = image_url_to_image_bytes(url)?;

    let img = image::load_from_memory(&image)?;

    let result = get_hex_code_from_img(img)?;

    Ok(result)
}

pub fn image_primary_color_by_base64(base64: &str) -> Result<String> {
    let bytes = BASE64_STANDARD.decode(base64)?;

    let img = image::load_from_memory(&bytes)?;

    let result = get_hex_code_from_img(img)?;

    Ok(result)
}



pub fn image_url_to_image_bytes(url: &str) -> Result<Vec<u8>> {

    let client = Client::new();

    let response = client.get(url).send()?;

    let bytes = response.bytes()?.to_vec();

    Ok(bytes)
}