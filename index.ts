const bindings = require('../build/Release/primary-color');

export const primaryImageColorFromUrl = (url: string, format: string): string => {
    return bindings.primaryColorByImageUrl(url, format);
};

export const primaryImageColorFromBase64 = (base64: string, format: string): string => {
    return bindings.primaryColorByBase64(base64, format);
};