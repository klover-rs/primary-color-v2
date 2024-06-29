pub fn convert_to_vec(input: &str) -> Vec<String> {
    input
        .trim_matches(|c| c == '[' || c == ']')
        .split(", ")
        .map(|s| s.to_string())
        .collect()
}

