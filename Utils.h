#pragma once
// Shared helpers: console input, validation and parcel printing.

#include <string>
#include "Models.h"

std::string trim(const std::string& s);
std::string toUpper(std::string s);

// Prompts and reads one trimmed line. Returns false at end of input.
bool readLine(const std::string& prompt, std::string& out);
// Returns -1 if the input is not a whole number
int  readChoice(const std::string& prompt);

bool isAlnum(char c);
// YYYY-MM-DD HH:MM
bool validDateTime(const std::string& s);

void printHeader();
void printRow(const Parcel& p);
void printDetail(const Parcel& p);

bool chooseType(ParcelType& out);
bool chooseStatus(ParcelStatus& out);
