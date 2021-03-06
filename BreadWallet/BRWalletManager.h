//
//  BRWalletManager.h
//  BreadWallet
//
//  Created by Aaron Voisine on 3/2/14.
//  Copyright (c) 2014 Aaron Voisine <voisine@gmail.com>
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#import <Foundation/Foundation.h>

#define WALLET_NEEDS_BACKUP_KEY @"WALLET_NEEDS_BACKUP"
#define BRWalletManagerSeedChangedNotification @"BRWalletManagerSeedChangedNotification"

@class BRWallet, BRTransaction;
@protocol BRKeySequence;

@interface BRWalletManager : NSObject<UIAlertViewDelegate>

@property (nonatomic, readonly) BRWallet *wallet;
@property (nonatomic, readonly) id<BRKeySequence> sequence;
@property (nonatomic, readonly) NSData *masterPublicKey; // master public key used to generate wallet addresses
@property (nonatomic, readonly) NSData *seed; // requesting seed will trigger authentication
@property (nonatomic, copy) NSString *seedPhrase; // requesting seedPhrase will trigger authentication
@property (nonatomic, readonly) NSTimeInterval seedCreationTime; // interval since refrence date, 00:00:00 01/01/01 GMT
@property (nonatomic, readonly, getter=isPasscodeEnabled) BOOL passcodeEnabled; // true if device passcode is enabled
@property (nonatomic, assign) BOOL didAuthenticate; // true if the user authenticated after this was last set to false
@property (nonatomic, readonly) NSNumberFormatter *format; // auroracoin currency formatter
@property (nonatomic, readonly) NSNumberFormatter *localFormat; // local currency formatter
@property (nonatomic, copy) NSString *localCurrencyCode; // local currency ISO code
@property (nonatomic, readonly) double localCurrencyPrice; // exchange rate in local currency units per auroracoin
@property (nonatomic, readonly) NSArray *currencyCodes; // list of supported local currencies

+ (instancetype)sharedInstance;

- (NSString *)generateRandomSeed; // generates a random seed, saves to keychain and returns the associated seedPhrase
- (NSString *)seedPhraseWithPrompt:(NSString *)authprompt; // authenticates user and returns seedPhrase
- (NSData *)seedWithPrompt:(NSString *)authprompt; // authenticates user and returns seed
- (BOOL)authenticateWithPrompt:(NSString *)authprompt; // prompts user to authenticate with touch id or passcode

// given a private key, queries blockchain for unspent outputs and calls the completion block with a signed transaction
// that will sweep the balance into wallet (doesn't publish the tx)
- (void)sweepPrivateKey:(NSString *)privKey withFee:(BOOL)fee
completion:(void (^)(BRTransaction *tx, NSError *error))completion;

- (int64_t)amountForString:(NSString *)string;
- (NSString *)stringForAmount:(int64_t)amount;
- (int64_t)amountForLocalCurrencyString:(NSString *)string;

@end
