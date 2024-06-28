
use crate::image::get_image_buffer;
use anyhow::Result;

#[derive(Debug, Clone, Copy)]
struct RGB {
    r: u8,
    g: u8,
    b: u8
}


impl RGB {
    fn average_color(colors: Vec<RGB>) -> RGB {
        let mut sum_r = 0;
        let mut sum_g = 0;
        let mut sum_b = 0;

        let count = colors.len() as u32;

        for color in colors {
            sum_r += color.r as u32;
            sum_g += color.g as u32;
            sum_b += color.b as u32;
        }

        RGB {
            r: (sum_r / count) as u8,
            g: (sum_g / count) as u8,
            b: (sum_b / count) as u8,
        }
    }

    fn to_hex(&self) -> String {
        format!("#{:02X}{:02X}{:02X}", self.r, self.g, self.b)
    }
}

pub fn get_hex_code_from_img(img: image::DynamicImage) -> Result<String> {
    let (buffer, color_type) = get_image_buffer(img);

    let colors = color_thief::get_palette(&buffer, color_type, 10, 10)?;

    let mut rbg_color = vec![RGB { r: 0, g: 0, b: 0 }];

    for color in colors {
        let r = color.r;
        let g = color.g;
        let b = color.b;

        rbg_color.push(RGB { r: r, g: g, b: b })
    }

    rbg_color.remove(0);

    let avg_color = RGB::average_color(rbg_color);

    Ok(avg_color.to_hex())
}