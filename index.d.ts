export type XXHashInputData = string | Buffer;

export type XXHashOutputEncoding = "hex" | "base64" | "base64-u";

export interface XXHash {

	update(data: XXHashInputData): this;

	digest(): Buffer;

	digest(encoding: XXHashOutputEncoding): string;
}

export function createXXH3_128(): XXHash;

export function xxHash3_128(data: XXHashInputData): Buffer;

export function xxHash3_128(data: XXHashInputData, encoding: XXHashOutputEncoding): string;
