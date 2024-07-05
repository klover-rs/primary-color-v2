const bindings = require('./build/Release/primary-color');

module.exports = {
    primaryImageColorFromUrl: (url, format) => {
        return bindings.primaryColorByImageUrl(url, format);
    },
    primaryImageColorFromBase64: (base64, format) => {
        return bindings.primaryColorByBase64(base64, format);
    }
};