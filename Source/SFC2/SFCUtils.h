#pragma once

#define DEBUGMSG_FSTRING(STR) do { \
                                if (GEngine && GEngine->IsEditor()) \
                                  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, (STR)); \
                              } while (false)

#define DEBUGMSG(TXT) do { \
                        if (GEngine && GEngine->IsEditor()) { \
                          static const FString __ScrollingMessage(TEXT(TXT)); \
                          GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, __ScrollingMessage); \
                        } \
                      } while (false)

#define CHECK(expr) do { if ((expr)) {} else { UE_LOG(LogTemp, Fatal, TEXT("Expression (" #expr ") is false.")) } } while (false)
#define CHECKMSG(expr, msg) do { \
								if ((expr)) { \
								} \
								else { \
										UE_LOG(LogTemp, Fatal, TEXT("Expression (" #expr ") is false: " msg)); \
								} \
						    } while (false);

#define ECC_SFCWeaponTraceChannel ECC_GameTraceChannel2