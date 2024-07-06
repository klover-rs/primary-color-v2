declare module 'primary-color' {
    export function primaryColorByImageUrl(url: string, format: string): string;
    export function primaryColorByBase64(base64: string, format: string): string;
}