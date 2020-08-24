export type XXHashInputData = string | Buffer;
export type XXHashOutputEncoding = "hex" | "base64" | "base64-u";

export function xxHash3_128(data: XXHashInputData): Buffer;

export function xxHash3_128(data: XXHashInputData, encoding: XXHashOutputEncoding): string;
