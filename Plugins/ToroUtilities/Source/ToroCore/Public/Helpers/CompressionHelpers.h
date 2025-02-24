// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Compression/OodleDataCompressionUtil.h"

namespace CompressionHelpers
{
	static bool OodleCompress(const TArray<uint8>& InData, TArray<uint8>& OutData)
	{
		if (InData.IsEmpty()) return true;
		return FOodleCompressedArray::CompressTArray(OutData, InData,
			FOodleDataCompression::ECompressor::Kraken,
			FOodleDataCompression::ECompressionLevel::SuperFast);
	}

	static bool OodleDecompress(const TArray<uint8>& InData, TArray<uint8>& OutData)
	{
		if (InData.IsEmpty()) return true;
		return FOodleCompressedArray::DecompressToTArray(OutData, InData);
	}
}
