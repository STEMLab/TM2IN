/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 *
 * Tutorial 4: 3d engine - 3ds models loader
 * 
 * Include File: endianconverter.h
 *
 */
 
/**********************************************************
 *
 * This file contains some self-descriptive functions that all change the
 * endianness of the variable passed to them and return it.
 *
 *********************************************************/
 
 float ConvertEndianf(float infloat)
 {
	union floatunion { unsigned char part[4]; float float_value; } Converter;
	char swap_char;
	
	Converter.float_value = infloat;
	swap_char = Converter.part[0]; Converter.part[0] = Converter.part[3]; Converter.part[3] = swap_char;
	swap_char = Converter.part[1]; Converter.part[1] = Converter.part[2]; Converter.part[2] = swap_char;
	
	return Converter.float_value;
 }
 
 unsigned long ConvertEndianul(unsigned long inlong)
 {
 	union longunion { unsigned char part[4]; unsigned long long_value; } Converter;
	char swap_char;
	
	Converter.long_value = inlong;
	swap_char = Converter.part[0]; Converter.part[0] = Converter.part[3]; Converter.part[3] = swap_char;
	swap_char = Converter.part[1]; Converter.part[1] = Converter.part[2]; Converter.part[2] = swap_char;
	
	return Converter.long_value;
 }
 
 unsigned short ConvertEndianus(unsigned short inshort)
 {
	union shortunion { unsigned char part[2]; unsigned short short_value; } Converter;
	char swap_char;
	
	Converter.short_value = inshort;
	swap_char = Converter.part[0]; Converter.part[0] = Converter.part[1]; Converter.part[1] = swap_char;
	
	return Converter.short_value;
 }
