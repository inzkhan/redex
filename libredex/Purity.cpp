/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Purity.h"

static const char* pure_method_names[] = {
    "Ljava/lang/Boolean;.booleanValue:()Z",
    "Ljava/lang/Boolean;.equals:(Ljava/lang/Object;)Z",
    "Ljava/lang/Boolean;.getBoolean:(Ljava/lang/String;)Z",
    "Ljava/lang/Boolean;.hashCode:()I",
    "Ljava/lang/Boolean;.toString:()Ljava/lang/String;",
    "Ljava/lang/Boolean;.toString:(Z)Ljava/lang/String;",
    "Ljava/lang/Boolean;.valueOf:(Z)Ljava/lang/Boolean;",
    "Ljava/lang/Boolean;.valueOf:(Ljava/lang/String;)Ljava/lang/Boolean;",
    "Ljava/lang/Byte;.byteValue:()B",
    "Ljava/lang/Byte;.equals:(Ljava/lang/Object;)Z",
    "Ljava/lang/Byte;.toString:()Ljava/lang/String;",
    "Ljava/lang/Byte;.toString:(B)Ljava/lang/String;",
    "Ljava/lang/Byte;.valueOf:(B)Ljava/lang/Byte;",
    "Ljava/lang/Character;.valueOf:(C)Ljava/lang/Character;",
    "Ljava/lang/Character;.charValue:()C",
    "Ljava/lang/Class;.getName:()Ljava/lang/String;",
    "Ljava/lang/Class;.getSimpleName:()Ljava/lang/String;",
    "Ljava/lang/Double;.compare:(DD)I",
    "Ljava/lang/Double;.doubleValue:()D",
    "Ljava/lang/Double;.doubleToLongBits:(D)J",
    "Ljava/lang/Double;.doubleToRawLongBits:(D)J",
    "Ljava/lang/Double;.floatValue:()F",
    "Ljava/lang/Double;.hashCode:()I",
    "Ljava/lang/Double;.intValue:()I",
    "Ljava/lang/Double;.isInfinite:(D)Z",
    "Ljava/lang/Double;.isNaN:(D)Z",
    "Ljava/lang/Double;.longBitsToDouble:(J)D",
    "Ljava/lang/Double;.longValue:()J",
    "Ljava/lang/Double;.toString:(D)Ljava/lang/String;",
    "Ljava/lang/Double;.valueOf:(D)Ljava/lang/Double;",
    "Ljava/lang/Enum;.equals:(Ljava/lang/Object;)Z",
    "Ljava/lang/Enum;.name:()Ljava/lang/String;",
    "Ljava/lang/Enum;.ordinal:()I",
    "Ljava/lang/Enum;.toString:()Ljava/lang/String;",
    "Ljava/lang/Float;.doubleValue:()D",
    "Ljava/lang/Float;.floatToRawIntBits:(F)I",
    "Ljava/lang/Float;.floatValue:()F",
    "Ljava/lang/Float;.compare:(FF)I",
    "Ljava/lang/Float;.equals:(Ljava/lang/Object;)Z",
    "Ljava/lang/Float;.hashCode:()I",
    "Ljava/lang/Float;.intBitsToFloat:(I)F",
    "Ljava/lang/Float;.intValue:()I",
    "Ljava/lang/Float;.floatToIntBits:(F)I",
    "Ljava/lang/Float;.isInfinite:(F)Z",
    "Ljava/lang/Float;.isNaN:(F)Z",
    "Ljava/lang/Float;.valueOf:(F)Ljava/lang/Float;",
    "Ljava/lang/Float;.toString:(F)Ljava/lang/String;",
    "Ljava/lang/Integer;.bitCount:(I)I",
    "Ljava/lang/Integer;.byteValue:()B",
    "Ljava/lang/Integer;.compareTo:(Ljava/lang/Integer;)I",
    "Ljava/lang/Integer;.doubleValue:()D",
    "Ljava/lang/Integer;.equals:(Ljava/lang/Object;)Z",
    "Ljava/lang/Integer;.hashCode:()I",
    "Ljava/lang/Integer;.highestOneBit:(I)I",
    "Ljava/lang/Integer;.intValue:()I",
    "Ljava/lang/Integer;.longValue:()J",
    "Ljava/lang/Integer;.lowestOneBit:(I)I",
    "Ljava/lang/Integer;.numberOfLeadingZeros:(I)I",
    "Ljava/lang/Integer;.numberOfTrailingZeros:(I)I",
    "Ljava/lang/Integer;.shortValue:()S",
    "Ljava/lang/Integer;.signum:(I)I",
    "Ljava/lang/Integer;.toBinaryString:(I)Ljava/lang/String;",
    "Ljava/lang/Integer;.toHexString:(I)Ljava/lang/String;",
    "Ljava/lang/Integer;.toString:()Ljava/lang/String;",
    "Ljava/lang/Integer;.toString:(I)Ljava/lang/String;",
    "Ljava/lang/Integer;.toString:(II)Ljava/lang/String;",
    "Ljava/lang/Integer;.valueOf:(I)Ljava/lang/Integer;",
    "Ljava/lang/Long;.bitCount:(J)I",
    "Ljava/lang/Long;.compareTo:(Ljava/lang/Long;)I",
    "Ljava/lang/Long;.doubleValue:()D",
    "Ljava/lang/Long;.equals:(Ljava/lang/Object;)Z",
    "Ljava/lang/Long;.hashCode:()I",
    "Ljava/lang/Long;.intValue:()I",
    "Ljava/lang/Long;.highestOneBit:(J)J",
    "Ljava/lang/Long;.longValue:()J",
    "Ljava/lang/Long;.numberOfTrailingZeros:(J)I",
    "Ljava/lang/Long;.signum:(J)I",
    "Ljava/lang/Long;.toBinaryString:(J)Ljava/lang/String;",
    "Ljava/lang/Long;.toHexString:(J)Ljava/lang/String;",
    "Ljava/lang/Long;.toString:()Ljava/lang/String;",
    "Ljava/lang/Long;.toString:(J)Ljava/lang/String;",
    "Ljava/lang/Long;.valueOf:(J)Ljava/lang/Long;",
    "Ljava/lang/Math;.IEEEremainder:(DD)D",
    "Ljava/lang/Math;.abs:(J)J",
    "Ljava/lang/Math;.abs:(I)I",
    "Ljava/lang/Math;.abs:(F)F",
    "Ljava/lang/Math;.abs:(D)D",
    "Ljava/lang/Math;.acos:(D)D",
    "Ljava/lang/Math;.asin:(D)D",
    "Ljava/lang/Math;.atan:(D)D",
    "Ljava/lang/Math;.atan2:(DD)D",
    "Ljava/lang/Math;.cbrt:(D)D",
    "Ljava/lang/Math;.ceil:(D)D",
    "Ljava/lang/Math;.copySign:(FF)F",
    "Ljava/lang/Math;.copySign:(DD)D",
    "Ljava/lang/Math;.cos:(D)D",
    "Ljava/lang/Math;.cosh:(D)D",
    "Ljava/lang/Math;.exp:(D)D",
    "Ljava/lang/Math;.expm1:(D)D",
    "Ljava/lang/Math;.floor:(D)D",
    "Ljava/lang/Math;.floorDiv:(II)I",
    "Ljava/lang/Math;.floorDiv:(JJ)J",
    "Ljava/lang/Math;.floorMod:(JJ)J",
    "Ljava/lang/Math;.floorMod:(II)I",
    "Ljava/lang/Math;.getExponent:(D)I",
    "Ljava/lang/Math;.getExponent:(F)I",
    "Ljava/lang/Math;.hypot:(DD)D",
    "Ljava/lang/Math;.log:(D)D",
    "Ljava/lang/Math;.log10:(D)D",
    "Ljava/lang/Math;.log1p:(D)D",
    "Ljava/lang/Math;.max:(II)I",
    "Ljava/lang/Math;.max:(JJ)J",
    "Ljava/lang/Math;.max:(FF)F",
    "Ljava/lang/Math;.max:(DD)D",
    "Ljava/lang/Math;.min:(FF)F",
    "Ljava/lang/Math;.min:(DD)D",
    "Ljava/lang/Math;.min:(II)I",
    "Ljava/lang/Math;.min:(JJ)J",
    "Ljava/lang/Math;.nextAfter:(DD)D",
    "Ljava/lang/Math;.nextAfter:(FD)F",
    "Ljava/lang/Math;.nextDown:(D)D",
    "Ljava/lang/Math;.nextDown:(F)F",
    "Ljava/lang/Math;.nextUp:(F)F",
    "Ljava/lang/Math;.nextUp:(D)D",
    "Ljava/lang/Math;.pow:(DD)D",
    "Ljava/lang/Math;.random:()D",
    "Ljava/lang/Math;.rint:(D)D",
    "Ljava/lang/Math;.round:(D)J",
    "Ljava/lang/Math;.round:(F)I",
    "Ljava/lang/Math;.scalb:(FI)F",
    "Ljava/lang/Math;.scalb:(DI)D",
    "Ljava/lang/Math;.signum:(D)D",
    "Ljava/lang/Math;.signum:(F)F",
    "Ljava/lang/Math;.sin:(D)D",
    "Ljava/lang/Math;.sinh:(D)D",
    "Ljava/lang/Math;.sqrt:(D)D",
    "Ljava/lang/Math;.tan:(D)D",
    "Ljava/lang/Math;.tanh:(D)D",
    "Ljava/lang/Math;.toDegrees:(D)D",
    "Ljava/lang/Math;.toRadians:(D)D",
    "Ljava/lang/Math;.ulp:(D)D",
    "Ljava/lang/Math;.ulp:(F)F",
    "Ljava/lang/Object;.getClass:()Ljava/lang/Class;",
    "Ljava/lang/Short;.equals:(Ljava/lang/Object;)Z",
    "Ljava/lang/Short;.shortValue:()S",
    "Ljava/lang/Short;.toString:(S)Ljava/lang/String;",
    "Ljava/lang/Short;.valueOf:(S)Ljava/lang/Short;",
    "Ljava/lang/String;.compareTo:(Ljava/lang/String;)I",
    "Ljava/lang/String;.compareToIgnoreCase:(Ljava/lang/String;)I",
    "Ljava/lang/String;.concat:(Ljava/lang/String;)Ljava/lang/String;",
    "Ljava/lang/String;.endsWith:(Ljava/lang/String;)Z",
    "Ljava/lang/String;.equals:(Ljava/lang/Object;)Z",
    "Ljava/lang/String;.equalsIgnoreCase:(Ljava/lang/String;)Z",
    "Ljava/lang/String;.hashCode:()I",
    "Ljava/lang/String;.indexOf:(I)I",
    "Ljava/lang/String;.isEmpty:()Z",
    "Ljava/lang/String;.indexOf:(Ljava/lang/String;)I",
    "Ljava/lang/String;.indexOf:(II)I",
    "Ljava/lang/String;.indexOf:(Ljava/lang/String;I)I",
    "Ljava/lang/String;.lastIndexOf:(I)I",
    "Ljava/lang/String;.lastIndexOf:(II)I",
    "Ljava/lang/String;.lastIndexOf:(Ljava/lang/String;)I",
    "Ljava/lang/String;.lastIndexOf:(Ljava/lang/String;I)I",
    "Ljava/lang/String;.length:()I",
    "Ljava/lang/String;.replace:(CC)Ljava/lang/String;",
    "Ljava/lang/String;.startsWith:(Ljava/lang/String;)Z",
    "Ljava/lang/String;.startsWith:(Ljava/lang/String;I)Z",
    "Ljava/lang/String;.toLowerCase:()Ljava/lang/String;",
    "Ljava/lang/String;.toLowerCase:(Ljava/util/Locale;)Ljava/lang/String;",
    "Ljava/lang/String;.toString:()Ljava/lang/String;",
    "Ljava/lang/String;.toUpperCase:()Ljava/lang/String;",
    "Ljava/lang/String;.toUpperCase:(Ljava/util/Locale;)Ljava/lang/String;",
    "Ljava/lang/String;.trim:()Ljava/lang/String;",
    "Ljava/lang/String;.valueOf:(C)Ljava/lang/String;",
    "Ljava/lang/String;.valueOf:(D)Ljava/lang/String;",
    "Ljava/lang/String;.valueOf:(F)Ljava/lang/String;",
    "Ljava/lang/String;.valueOf:(I)Ljava/lang/String;",
    "Ljava/lang/String;.valueOf:(J)Ljava/lang/String;",
    "Ljava/lang/String;.valueOf:(Z)Ljava/lang/String;",
    "Ljava/lang/System;.identityHashCode:(Ljava/lang/Object;)I",
    "Ljava/lang/Thread;.currentThread:()Ljava/lang/Thread;",
};

std::unordered_set<DexMethodRef*> get_pure_methods() {
  std::unordered_set<DexMethodRef*> pure_methods;
  for (auto const pure_method_name : pure_method_names) {
    auto method_ref = DexMethod::get_method(std::string(pure_method_name));
    if (method_ref == nullptr) {
      TRACE(CSE, 1, "[get_pure_methods]: Could not find pure method %s",
            pure_method_name);
      continue;
    }

    pure_methods.insert(method_ref);
  }
  return pure_methods;
}