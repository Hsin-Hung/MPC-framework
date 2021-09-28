#ifndef MPCTYPES_H
#define MPCTYPES_H

#include <limits.h>
#include <stdbool.h>

/**
 * Original data
 **/
typedef long long Data;

/**
 * Seed for random number generator
 **/
typedef long long Seed;

/**
 * Arithmetic Share
 **/
typedef long long AShare;

/**
 * Boolean Share
 **/
typedef long long BShare;

/**
 * Boolean single-bit Share
 **/
typedef bool BitShare;

/**
 * AShareTable represents an arithmetic share of an input Table
 * that was produced by a data owner with ownerId
 * and was assigned to the computation party with partyId.
**/
typedef struct {
    int ownerId;
    int partyId;
    int numRows;
    int numCols;
    int relationId;
    AShare **contents;
} AShareTable;

/**
 * BShareTable represents a boolean share of an input Table
 * that was produced by a data owner with ownerId
 * and was assigned to the computation party with partyId.
**/
typedef struct {
    int ownerId;
    int partyId;
    int numRows;
    int numCols;
    int relationId;
    BShare **contents;
} BShareTable;

/**
 * Table represents a relational table provided by the data owner with  ownerId.
**/
typedef struct {
    int ownerId;
    int numRows;
    int numCols;
    Data **contents;
} Table;

/**
 * PartyMessage represents a data message exchanges between data owners.
**/
typedef struct {
    int relationId;
    int rowNumber;
    // Payload p; //TODO: what's the type of Payload?
} PartyMessage;


/**
 * A pair of shares for a random w that is used in multiplications and arithmetic equality.
 **/
typedef struct {
  AShare first;
  AShare second;
} WSharePair;

#endif
