// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2016-2018, The Karbowanec developers
// Copyright (c) 2018, The Newton Developers
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bytecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Bytecoin.  If not, see <http://www.gnu.org/licenses/>.

#include "Currency.h"
#include <cctype>
#include <math.h>

#include <boost/algorithm/string/trim.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/lexical_cast.hpp>

#include "../Common/Base58.h"
#include "../Common/int-util.h"
#include "../Common/StringTools.h"
#include "../crypto/crypto-ops.h"
#include "../crypto/crypto.h"
#include "Account.h"
#include "CryptoNoteBasicImpl.h"
#include "CryptoNoteFormatUtils.h"
#include "CryptoNoteTools.h"
#include "TransactionExtra.h"
#include "UpgradeDetector.h"

#undef ERROR

using namespace Logging;
using namespace Common;

namespace CryptoNote {

const std::vector<uint64_t> Currency::PRETTY_AMOUNTS = {
  1, 2, 3, 4, 5, 6, 7, 8, 9,
  10, 20, 30, 40, 50, 60, 70, 80, 90,
  100, 200, 300, 400, 500, 600, 700, 800, 900,
  1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
  10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
  100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000,
  1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000,
  10000000, 20000000, 30000000, 40000000, 50000000, 60000000, 70000000, 80000000, 90000000,
  100000000, 200000000, 300000000, 400000000, 500000000, 600000000, 700000000, 800000000, 900000000,
  1000000000, 2000000000, 3000000000, 4000000000, 5000000000, 6000000000, 7000000000, 8000000000, 9000000000,
  10000000000, 20000000000, 30000000000, 40000000000, 50000000000, 60000000000, 70000000000, 80000000000, 90000000000,
  100000000000, 200000000000, 300000000000, 400000000000, 500000000000, 600000000000, 700000000000, 800000000000, 900000000000,
  1000000000000, 2000000000000, 3000000000000, 4000000000000, 5000000000000, 6000000000000, 7000000000000, 8000000000000, 9000000000000,
  10000000000000, 20000000000000, 30000000000000, 40000000000000, 50000000000000, 60000000000000, 70000000000000, 80000000000000, 90000000000000,
  100000000000000, 200000000000000, 300000000000000, 400000000000000, 500000000000000, 600000000000000, 700000000000000, 800000000000000, 900000000000000,
  1000000000000000, 2000000000000000, 3000000000000000, 4000000000000000, 5000000000000000, 6000000000000000, 7000000000000000, 8000000000000000, 9000000000000000,
  10000000000000000, 20000000000000000, 30000000000000000, 40000000000000000, 50000000000000000, 60000000000000000, 70000000000000000, 80000000000000000, 90000000000000000,
  100000000000000000, 200000000000000000, 300000000000000000, 400000000000000000, 500000000000000000, 600000000000000000, 700000000000000000, 800000000000000000, 900000000000000000,
  1000000000000000000, 2000000000000000000, 3000000000000000000, 4000000000000000000, 5000000000000000000, 6000000000000000000, 7000000000000000000, 8000000000000000000, 9000000000000000000,
  10000000000000000000ull
};

bool Currency::init() {
  if (!generateGenesisBlock()) {
    logger(ERROR, BRIGHT_RED) << "Failed to generate genesis block";
    return false;
  }

  try {
    cachedGenesisBlock->getBlockHash();
  } catch (std::exception& e) {
    logger(ERROR, BRIGHT_RED) << "Failed to get genesis block hash: " << e.what();
    return false;
  }

  if (isTestnet()) {
	  m_upgradeHeightV2 = 3;
	  m_zawyLWMA2DifficultyBlockIndex = 3;
	  m_zawyLWMA3DifficultyBlockIndex = 6;
	  m_governancePercent = 20;
	  m_governanceHeightStart = 10;
	  m_governanceHeightEnd = 20;
    m_upgradeHeightV3 = 4294967294; //(static_cast<uint32_t>(-1) - 1);
    m_blocksFileName = "testnet_" + m_blocksFileName;
    m_blockIndexesFileName = "testnet_" + m_blockIndexesFileName;
    m_txPoolFileName = "testnet_" + m_txPoolFileName;
  }

  return true;
}

bool Currency::generateGenesisBlock() {
  genesisBlockTemplate = boost::value_initialized<BlockTemplate>();

  //account_public_address ac = boost::value_initialized<AccountPublicAddress>();
  //std::vector<size_t> sz;
  //constructMinerTx(0, 0, 0, 0, 0, ac, m_genesisBlock.baseTransaction); // zero fee in genesis
  //BinaryArray txb = toBinaryArray(m_genesisBlock.baseTransaction);
  //std::string hex_tx_represent = Common::toHex(txb);

  // Hard code coinbase tx in genesis block, because through generating tx use random, but genesis should be always the same
  std::string genesisCoinbaseTxHex = "013c01ff0001ffffffffffff0f029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd088071210108a042c825cb48ef6674212046be59394ef8bbcb2fe37cffcd332ec1dff224c4";
  BinaryArray minerTxBlob;

  bool r =
    fromHex(genesisCoinbaseTxHex, minerTxBlob) &&
    fromBinaryArray(genesisBlockTemplate.baseTransaction, minerTxBlob);

  if (!r) {
    logger(ERROR, BRIGHT_RED) << "failed to parse coinbase tx from hard coded blob";
    return false;
  }

  genesisBlockTemplate.majorVersion = BLOCK_MAJOR_VERSION_1;
  genesisBlockTemplate.minorVersion = BLOCK_MINOR_VERSION_0;
  genesisBlockTemplate.timestamp = 0;
  genesisBlockTemplate.nonce = 70;
  if (m_testnet) {
    ++genesisBlockTemplate.nonce;
  }
  //miner::find_nonce_for_given_block(bl, 1, 0);
  cachedGenesisBlock.reset(new CachedBlock(genesisBlockTemplate));
  return true;
}

size_t Currency::difficultyWindowByBlockVersion(uint8_t blockMajorVersion) const {
  if (blockMajorVersion >= BLOCK_MAJOR_VERSION_3) {
    return m_difficultyWindow;
  } else if (blockMajorVersion == BLOCK_MAJOR_VERSION_2) {
    return CryptoNote::parameters::DIFFICULTY_WINDOW_V2;
  } else {
    return CryptoNote::parameters::DIFFICULTY_WINDOW_V1;
  }
}

size_t Currency::difficultyLagByBlockVersion(uint8_t blockMajorVersion) const {
  if (blockMajorVersion >= BLOCK_MAJOR_VERSION_3) {
    return m_difficultyLag;
  } else if (blockMajorVersion == BLOCK_MAJOR_VERSION_2) {
    return CryptoNote::parameters::DIFFICULTY_LAG_V2;
  } else {
    return CryptoNote::parameters::DIFFICULTY_LAG_V1;
  }
}

size_t Currency::difficultyCutByBlockVersion(uint8_t blockMajorVersion) const {
  if (blockMajorVersion >= BLOCK_MAJOR_VERSION_3) {
    return m_difficultyCut;
  } else if (blockMajorVersion == BLOCK_MAJOR_VERSION_2) {
    return CryptoNote::parameters::DIFFICULTY_CUT_V2;
  } else {
    return CryptoNote::parameters::DIFFICULTY_CUT_V1;
  }
}

size_t Currency::difficultyBlocksCountByBlockVersion(uint8_t blockMajorVersion, uint32_t height) const {

	if (height >= m_zawyLWMA2DifficultyBlockIndex)
    {
        return CryptoNote::parameters::DIFFICULTY_BLOCKS_COUNT_V2;
    }

  return difficultyWindowByBlockVersion(blockMajorVersion) + difficultyLagByBlockVersion(blockMajorVersion);
}

size_t Currency::blockGrantedFullRewardZoneByBlockVersion(uint8_t blockMajorVersion) const {
  if (blockMajorVersion >= BLOCK_MAJOR_VERSION_3) {
    return m_blockGrantedFullRewardZone;
  } else if (blockMajorVersion == BLOCK_MAJOR_VERSION_2) {
    return CryptoNote::parameters::CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2;
  } else {
    return CryptoNote::parameters::CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1;
  }
}

uint32_t Currency::upgradeHeight(uint8_t majorVersion) const {
  if(majorVersion == BLOCK_MAJOR_VERSION_2) {
    return m_upgradeHeightV2;
  } else if (majorVersion == BLOCK_MAJOR_VERSION_3) {
    return m_upgradeHeightV3;
  } else {
    return static_cast<uint32_t>(-1);
  }
}

bool Currency::getBlockReward(uint8_t blockMajorVersion, size_t medianSize, size_t currentBlockSize, uint64_t alreadyGeneratedCoins,
  uint64_t fee, uint64_t& reward, int64_t& emissionChange) const {
  assert(alreadyGeneratedCoins <= m_moneySupply);
  assert(m_emissionSpeedFactor > 0 && m_emissionSpeedFactor <= 8 * sizeof(uint64_t));

  uint64_t baseReward = (m_moneySupply - alreadyGeneratedCoins) >> m_emissionSpeedFactor;

  size_t blockGrantedFullRewardZone = blockGrantedFullRewardZoneByBlockVersion(blockMajorVersion);
  medianSize = std::max(medianSize, blockGrantedFullRewardZone);
  if (currentBlockSize > UINT64_C(2) * medianSize) {
    logger(TRACE) << "Block cumulative size is too big: " << currentBlockSize << ", expected less than " << 2 * medianSize;
    return false;
  }

  uint64_t penalizedBaseReward = getPenalizedAmount(baseReward, medianSize, currentBlockSize);
  uint64_t penalizedFee = blockMajorVersion >= BLOCK_MAJOR_VERSION_2 ? getPenalizedAmount(fee, medianSize, currentBlockSize) : fee;
if (cryptonoteCoinVersion() == 1) {
  penalizedFee = getPenalizedAmount(fee, medianSize, currentBlockSize);
}

  emissionChange = penalizedBaseReward - (fee - penalizedFee);
  reward = penalizedBaseReward + penalizedFee;

  return true;
}

size_t Currency::maxBlockCumulativeSize(uint64_t height) const {
  assert(height <= std::numeric_limits<uint64_t>::max() / m_maxBlockSizeGrowthSpeedNumerator);
  size_t maxSize = static_cast<size_t>(m_maxBlockSizeInitial +
    (height * m_maxBlockSizeGrowthSpeedNumerator) / m_maxBlockSizeGrowthSpeedDenominator);
  assert(maxSize >= m_maxBlockSizeInitial);
  return maxSize;
}

bool Currency::isGovernanceEnabled(uint32_t height) const {

	if (height >= m_governanceHeightStart && height <= m_governanceHeightEnd) {
		return true;
	}
	else
	{
		return false;
	}
}

// governance reward is 20 % of block reward
uint64_t Currency::getGovernanceReward(uint64_t base_reward) const {

	// minimum is 1% to avoid zero amount and maximum is 50%
	uint16_t percent = (m_governancePercent < 1) ? 1 : (m_governancePercent > 50) ? 50 : m_governancePercent;
	return (uint64_t)(base_reward * (percent * 0.01));
}

bool Currency::getGovernanceAddressAndKey(AccountKeys& governanceKeys) const
{
  std::string address;
  std::string viewSecretkey;

  if (isTestnet())
  {
	address = TESTNET_GOVERNANCE_WALLET_ADDRESS;
	viewSecretkey = TESTNET_GOVERNANCE_VIEW_SECRET_KEY;
  }
  else
  {
	address = GOVERNANCE_WALLET_ADDRESS;
	viewSecretkey = GOVERNANCE_VIEW_SECRET_KEY;
  }

  AccountPublicAddress governanceAddress = boost::value_initialized<AccountPublicAddress>();
  if (!parseAccountAddressString(address, governanceAddress)) {
	logger(Logging::ERROR) << "failed to parse governance wallet address";
	return false;
  }

  Crypto::SecretKey governanceViewSecretKey;
  if (!Common::podFromHex(viewSecretkey, governanceViewSecretKey)) {
	logger(Logging::ERROR) << "failed to parse governance view secret key";
	return false;
  }

  governanceKeys.address = governanceAddress;
  governanceKeys.viewSecretKey = governanceViewSecretKey;

  return true;
}

bool Currency::constructMinerTx(uint8_t blockMajorVersion, uint32_t height, size_t medianSize, uint64_t alreadyGeneratedCoins, size_t currentBlockSize,
  uint64_t fee, const AccountPublicAddress& minerAddress, Transaction& tx, const BinaryArray& extraNonce/* = BinaryArray()*/, size_t maxOuts/* = 1*/) const {

  tx.inputs.clear();
  tx.outputs.clear();
  tx.extra.clear();

  KeyPair txkey = generateKeyPair();
  addTransactionPublicKeyToExtra(tx.extra, txkey.publicKey);
  if (!extraNonce.empty()) {
	  if (!addExtraNonceToTransactionExtra(tx.extra, extraNonce)) {
		  return false;
	  }
  }

  BaseInput in;
  in.blockIndex = height;

  uint64_t governanceReward = 0;
  uint64_t TotalReward = 0;
  uint64_t blockReward;
  int64_t emissionChange;
  if (!getBlockReward(blockMajorVersion, medianSize, currentBlockSize, alreadyGeneratedCoins, fee, blockReward, emissionChange)) {
    logger(INFO) << "Block is too big";
    return false;
  }

  TotalReward = blockReward;
  uint64_t summaryAmounts = 0;

  bool enable_Governace = isGovernanceEnabled(height);
  if (enable_Governace) {

	  governanceReward = getGovernanceReward(blockReward);

	  if (alreadyGeneratedCoins != 0)
	  {
		  blockReward -= governanceReward;
		  TotalReward  = blockReward + governanceReward;
	  }
  }

  std::vector<uint64_t> outAmounts;
  decompose_amount_into_digits(blockReward, m_defaultDustThreshold,
	  [&outAmounts](uint64_t a_chunk) { outAmounts.push_back(a_chunk); },
	  [&outAmounts](uint64_t a_dust) { outAmounts.push_back(a_dust); });

  if (!(1 <= maxOuts)) { logger(ERROR, BRIGHT_RED) << "max_out must be non-zero"; return false; }
  while (maxOuts < outAmounts.size()) {
	  outAmounts[outAmounts.size() - 2] += outAmounts.back();
	  outAmounts.resize(outAmounts.size() - 1);
  }

  for (size_t no = 0; no < outAmounts.size(); no++) {
	  Crypto::KeyDerivation derivation = boost::value_initialized<Crypto::KeyDerivation>();
	  Crypto::PublicKey outEphemeralPubKey = boost::value_initialized<Crypto::PublicKey>();

	  bool r = Crypto::generate_key_derivation(minerAddress.viewPublicKey, txkey.secretKey, derivation);

	  if (!(r)) {
		  logger(ERROR, BRIGHT_RED)
			  << "while creating outs: failed to generate_key_derivation("
			  << minerAddress.viewPublicKey << ", " << txkey.secretKey << ")";
		  return false;
	  }

	  r = Crypto::derive_public_key(derivation, no, minerAddress.spendPublicKey, outEphemeralPubKey);

	  if (!(r)) {
		  logger(ERROR, BRIGHT_RED)
			  << "while creating outs: failed to derive_public_key("
			  << derivation << ", " << no << ", "
			  << minerAddress.spendPublicKey << ")";
		  return false;
	  }

	  KeyOutput tk;
	  tk.key = outEphemeralPubKey;

	  TransactionOutput out;
	  summaryAmounts += out.amount = outAmounts[no];
	  out.target = tk;
	  tx.outputs.push_back(out);
  }

  if (enable_Governace) {

	  AccountKeys governanceKeys;
	  getGovernanceAddressAndKey(governanceKeys);

		  Crypto::KeyDerivation derivation = boost::value_initialized<Crypto::KeyDerivation>();
		  Crypto::PublicKey outEphemeralPubKey = boost::value_initialized<Crypto::PublicKey>();

		  bool r = Crypto::generate_key_derivation(governanceKeys.address.viewPublicKey, txkey.secretKey, derivation);
		  if (!(r)) {
			  logger(ERROR, BRIGHT_RED)
				  << "while creating outs: failed to generate_key_derivation("
				  << governanceKeys.address.viewPublicKey << ", " << txkey.secretKey << ")";
			  return false;
		  }
		  size_t pos = tx.outputs.size();
		  r = Crypto::derive_public_key(derivation, pos++, governanceKeys.address.spendPublicKey, outEphemeralPubKey);

		  if (!(r)) {
			  logger(ERROR, BRIGHT_RED)
				  << "while creating outs: failed to derive_public_key("
				  << derivation << ", " << 0 << ", "
				  << governanceKeys.address.spendPublicKey << ")";
			  return false;
		  }

		  KeyOutput tk;
		  tk.key = outEphemeralPubKey;

		  TransactionOutput out;
		  summaryAmounts += out.amount = governanceReward;
		  out.target = tk;
		  tx.outputs.push_back(out);
  }


  if (!(summaryAmounts == TotalReward)) {
	logger(ERROR, BRIGHT_RED) << "Failed to construct miner tx, summaryAmounts = " << summaryAmounts << " not equal blockReward = " << TotalReward;
	return false;
  }

  tx.version = CURRENT_TRANSACTION_VERSION;
  //lock
  tx.unlockTime = height + m_minedMoneyUnlockWindow;
  tx.inputs.push_back(in);
  return true;
}

bool Currency::isFusionTransaction(const std::vector<uint64_t>& inputsAmounts, const std::vector<uint64_t>& outputsAmounts, size_t size) const {
  if (size > fusionTxMaxSize()) {
    return false;
  }

  if (inputsAmounts.size() < fusionTxMinInputCount()) {
    return false;
  }

  if (inputsAmounts.size() < outputsAmounts.size() * fusionTxMinInOutCountRatio()) {
    return false;
  }

  uint64_t inputAmount = 0;
  for (auto amount: inputsAmounts) {
    if (amount < defaultDustThreshold()) {
      return false;
    }

    inputAmount += amount;
  }

  std::vector<uint64_t> expectedOutputsAmounts;
  expectedOutputsAmounts.reserve(outputsAmounts.size());
  decomposeAmount(inputAmount, defaultDustThreshold(), expectedOutputsAmounts);
  std::sort(expectedOutputsAmounts.begin(), expectedOutputsAmounts.end());

  return expectedOutputsAmounts == outputsAmounts;
}

bool Currency::isFusionTransaction(const Transaction& transaction, size_t size) const {
  assert(getObjectBinarySize(transaction) == size);

  std::vector<uint64_t> outputsAmounts;
  outputsAmounts.reserve(transaction.outputs.size());
  for (const TransactionOutput& output : transaction.outputs) {
    outputsAmounts.push_back(output.amount);
  }

  return isFusionTransaction(getInputsAmounts(transaction), outputsAmounts, size);
}

bool Currency::isFusionTransaction(const Transaction& transaction) const {
  return isFusionTransaction(transaction, getObjectBinarySize(transaction));
}

bool Currency::isAmountApplicableInFusionTransactionInput(uint64_t amount, uint64_t threshold) const {
  uint8_t ignore;
  return isAmountApplicableInFusionTransactionInput(amount, threshold, ignore);
}

bool Currency::isAmountApplicableInFusionTransactionInput(uint64_t amount, uint64_t threshold, uint8_t& amountPowerOfTen) const {
  if (amount >= threshold) {
    return false;
  }

  if (amount < defaultDustThreshold()) {
    return false;
  }

  auto it = std::lower_bound(PRETTY_AMOUNTS.begin(), PRETTY_AMOUNTS.end(), amount);
  if (it == PRETTY_AMOUNTS.end() || amount != *it) {
    return false;
  }

  amountPowerOfTen = static_cast<uint8_t>(std::distance(PRETTY_AMOUNTS.begin(), it) / 9);
  return true;
}

std::string Currency::accountAddressAsString(const AccountBase& account) const {
  return getAccountAddressAsStr(m_publicAddressBase58Prefix, account.getAccountKeys().address);
}

std::string Currency::accountAddressAsString(const AccountPublicAddress& accountPublicAddress) const {
  return getAccountAddressAsStr(m_publicAddressBase58Prefix, accountPublicAddress);
}

bool Currency::parseAccountAddressString(const std::string& str, AccountPublicAddress& addr) const {
  uint64_t prefix;
  if (!CryptoNote::parseAccountAddressString(prefix, addr, str)) {
    return false;
  }

  if (prefix != m_publicAddressBase58Prefix) {
    logger(DEBUGGING) << "Wrong address prefix: " << prefix << ", expected " << m_publicAddressBase58Prefix;
    return false;
  }

  return true;
}

std::string Currency::formatAmount(uint64_t amount) const {
  std::string s = std::to_string(amount);
  if (s.size() < m_numberOfDecimalPlaces + 1) {
    s.insert(0, m_numberOfDecimalPlaces + 1 - s.size(), '0');
  }
  s.insert(s.size() - m_numberOfDecimalPlaces, ".");
  return s;
}

std::string Currency::formatAmount(int64_t amount) const {
  std::string s = formatAmount(static_cast<uint64_t>(std::abs(amount)));

  if (amount < 0) {
    s.insert(0, "-");
  }

  return s;
}

bool Currency::parseAmount(const std::string& str, uint64_t& amount) const {
  std::string strAmount = str;
  boost::algorithm::trim(strAmount);

  size_t pointIndex = strAmount.find_first_of('.');
  size_t fractionSize;
  if (std::string::npos != pointIndex) {
    fractionSize = strAmount.size() - pointIndex - 1;
    while (m_numberOfDecimalPlaces < fractionSize && '0' == strAmount.back()) {
      strAmount.erase(strAmount.size() - 1, 1);
      --fractionSize;
    }
    if (m_numberOfDecimalPlaces < fractionSize) {
      return false;
    }
    strAmount.erase(pointIndex, 1);
  } else {
    fractionSize = 0;
  }

  if (strAmount.empty()) {
    return false;
  }

  if (!std::all_of(strAmount.begin(), strAmount.end(), ::isdigit)) {
    return false;
  }

  if (fractionSize < m_numberOfDecimalPlaces) {
    strAmount.append(m_numberOfDecimalPlaces - fractionSize, '0');
  }

  return Common::fromString(strAmount, amount);
}

Difficulty Currency::nextDifficulty(std::vector<uint64_t> timestamps, std::vector<Difficulty> cumulativeDifficulties) const {
  assert(m_difficultyWindow >= 2);

  if (timestamps.size() > m_difficultyWindow) {
    timestamps.resize(m_difficultyWindow);
    cumulativeDifficulties.resize(m_difficultyWindow);
  }

  size_t length = timestamps.size();
  assert(length == cumulativeDifficulties.size());
  assert(length <= m_difficultyWindow);
  if (length <= 1) {
    return 1;
  }

  sort(timestamps.begin(), timestamps.end());

  size_t cutBegin, cutEnd;
  assert(2 * m_difficultyCut <= m_difficultyWindow - 2);
  if (length <= m_difficultyWindow - 2 * m_difficultyCut) {
    cutBegin = 0;
    cutEnd = length;
  } else {
    cutBegin = (length - (m_difficultyWindow - 2 * m_difficultyCut) + 1) / 2;
    cutEnd = cutBegin + (m_difficultyWindow - 2 * m_difficultyCut);
  }
  assert(/*cut_begin >= 0 &&*/ cutBegin + 2 <= cutEnd && cutEnd <= length);
  uint64_t timeSpan = timestamps[cutEnd - 1] - timestamps[cutBegin];
  if (timeSpan == 0) {
    timeSpan = 1;
  }

  Difficulty totalWork = cumulativeDifficulties[cutEnd - 1] - cumulativeDifficulties[cutBegin];
  assert(totalWork > 0);

  uint64_t low, high;
  low = mul128(totalWork, m_difficultyTarget, &high);
  if (high != 0 || std::numeric_limits<uint64_t>::max() - low < (timeSpan - 1)) {
    return 0;
  }

  return (low + timeSpan - 1) / timeSpan;
}

Difficulty Currency::getNextDifficulty(uint8_t version, uint32_t blockIndex, std::vector<uint64_t> timestamps, std::vector<Difficulty> cumulativeDifficulties) const {

    if (blockIndex >= m_zawyLWMA2DifficultyBlockIndex)
	{
		return nextDifficultyV2(version, blockIndex, timestamps, cumulativeDifficulties);
	}
	else
	{
		return nextDifficultyDefault(version, blockIndex, timestamps, cumulativeDifficulties);
	}
}

Difficulty Currency::nextDifficultyV2(uint8_t version, uint32_t blockIndex, std::vector<uint64_t> timestamps,
	std::vector<Difficulty> cumulativeDifficulties) const {

	// LWMA-2 / LWMA-3 difficulty algorithm 
	// Copyright (c) 2017-2018 Zawy, MIT License
	// https://github.com/zawy12/difficulty-algorithms/issues/3
	// with modifications by Ryo Currency developers

	int64_t T = static_cast<int64_t>(m_difficultyTarget); // DIFFICULTY_TARGET = 120	sec
	int64_t N = static_cast<int64_t>(m_zawyLWMA2DifficultyN); // DIFFICULTY_WINDOW_V2 = 60
	int64_t FTL = static_cast<int64_t>(m_blockFutureTimeLimitV2); // (3 * DIFFICULTY_TARGET)= 360 sec
	int64_t LWMA(0), solveTime(0), sum_3_ST(0);
	Difficulty next_D, prev_D;

	if (timestamps.size() > N + 1) {
		timestamps.resize(N + 1);
		cumulativeDifficulties.resize(N + 1);
	}

	size_t n = timestamps.size();
	assert(n == cumulativeDifficulties.size());
	assert(n <= N);
	if (n <= 1)
		return 1;

	uint64_t initial_difficulty_guess = 1000;
	if (timestamps.size() <= static_cast<uint64_t>(N)) {
		return initial_difficulty_guess;
	}

	int64_t max_TS, prev_max_TS;
	prev_max_TS = timestamps[0];
	uint32_t LWMA3_BLOCK_INDEX = m_zawyLWMA3DifficultyBlockIndex;

	// Loop through N most recent blocks.
	for (int64_t i = 1; i <= N; i++) {
		if (blockIndex < LWMA3_BLOCK_INDEX) { // LWMA-2
			solveTime = std::max(-FTL, std::min(static_cast<int64_t>(timestamps[i]) - static_cast<int64_t>(timestamps[i - 1]), 6 * T));
		}
		else { // LWMA-3
			if (static_cast<int64_t>(timestamps[i]) > prev_max_TS) {
				max_TS = timestamps[i];
			}
			else {
				max_TS = prev_max_TS + 1;
			}
			solveTime = std::min(6 * T, max_TS - prev_max_TS);
			prev_max_TS = max_TS;
		}

		LWMA += solveTime * i;
		if (i > N - 3) { sum_3_ST += solveTime; }
                }
		next_D = (static_cast<int64_t>(cumulativeDifficulties[N] - cumulativeDifficulties[0]) * T * (N + 1) * 99) / (100 * 2 * LWMA);
		prev_D = cumulativeDifficulties[N] - cumulativeDifficulties[N - 1];

		// Make sure we don't divide by zero if 50x attacker
		next_D = std::max((prev_D * 75) / 100, std::min(next_D, (prev_D * 133) / 100));

		if (sum_3_ST < (8 * T) / 10) {
			next_D = (prev_D * 110) / 100;
		

		return static_cast<uint64_t>(next_D);
	}
}


Difficulty Currency::nextDifficultyDefault(uint8_t version, uint32_t blockIndex, std::vector<uint64_t> timestamps, std::vector<Difficulty> cumulativeDifficulties) const {

  size_t c_difficultyWindow = difficultyWindowByBlockVersion(version);
  size_t c_difficultyCut = difficultyCutByBlockVersion(version);

  assert(c_difficultyWindow >= 2);

  if (timestamps.size() > c_difficultyWindow) {
    timestamps.resize(c_difficultyWindow);
    cumulativeDifficulties.resize(c_difficultyWindow);
  }

  size_t length = timestamps.size();
  assert(length == cumulativeDifficulties.size());
  assert(length <= c_difficultyWindow);
  if (length <= 1) {
    return 1;
  }

  sort(timestamps.begin(), timestamps.end());

  size_t cutBegin, cutEnd;
  assert(2 * c_difficultyCut <= c_difficultyWindow - 2);
  if (length <= c_difficultyWindow - 2 * c_difficultyCut) {
    cutBegin = 0;
    cutEnd = length;
  } else {
    cutBegin = (length - (c_difficultyWindow - 2 * c_difficultyCut) + 1) / 2;
    cutEnd = cutBegin + (c_difficultyWindow - 2 * c_difficultyCut);
  }
  assert(/*cut_begin >= 0 &&*/ cutBegin + 2 <= cutEnd && cutEnd <= length);
  uint64_t timeSpan = timestamps[cutEnd - 1] - timestamps[cutBegin];
  if (timeSpan == 0) {
    timeSpan = 1;
  }

  Difficulty totalWork = cumulativeDifficulties[cutEnd - 1] - cumulativeDifficulties[cutBegin];
  assert(totalWork > 0);

  uint64_t low, high;
  low = mul128(totalWork, m_difficultyTarget, &high);
  if (high != 0 || std::numeric_limits<uint64_t>::max() - low < (timeSpan - 1)) {
    return 0;
  }

  return (low + timeSpan - 1) / timeSpan;  // with version
}

bool Currency::checkProofOfWorkV1(Crypto::cn_context& context, const CachedBlock& block, Difficulty currentDifficulty) const {
  if (BLOCK_MAJOR_VERSION_1 != block.getBlock().majorVersion) {
    return false;
  }

  return check_hash(block.getBlockLongHash(context), currentDifficulty);
}

bool Currency::checkProofOfWorkV2(Crypto::cn_context& context, const CachedBlock& cachedBlock, Difficulty currentDifficulty) const {
  const auto& block = cachedBlock.getBlock();
  if (block.majorVersion < BLOCK_MAJOR_VERSION_2) {
    return false;
  }

  if (!check_hash(cachedBlock.getBlockLongHash(context), currentDifficulty)) {
    return false;
  }

  TransactionExtraMergeMiningTag mmTag;
  if (!getMergeMiningTagFromExtra(block.parentBlock.baseTransaction.extra, mmTag)) {
    logger(ERROR) << "merge mining tag wasn't found in extra of the parent block miner transaction";
    return false;
  }

  if (8 * sizeof(cachedGenesisBlock->getBlockHash()) < block.parentBlock.blockchainBranch.size()) {
    return false;
  }

  Crypto::Hash auxBlocksMerkleRoot;
  Crypto::tree_hash_from_branch(block.parentBlock.blockchainBranch.data(), block.parentBlock.blockchainBranch.size(),
    cachedBlock.getAuxiliaryBlockHeaderHash(), &cachedGenesisBlock->getBlockHash(), auxBlocksMerkleRoot);

  if (auxBlocksMerkleRoot != mmTag.merkleRoot) {
    logger(ERROR, BRIGHT_YELLOW) << "Aux block hash wasn't found in merkle tree";
    return false;
  }

  return true;
}

bool Currency::checkProofOfWork(Crypto::cn_context& context, const CachedBlock& block, Difficulty currentDiffic) const {
  switch (block.getBlock().majorVersion) {
  case BLOCK_MAJOR_VERSION_1:
    return checkProofOfWorkV1(context, block, currentDiffic);

  case BLOCK_MAJOR_VERSION_2:
  case BLOCK_MAJOR_VERSION_3:
    return checkProofOfWorkV2(context, block, currentDiffic);
  }

  logger(ERROR, BRIGHT_RED) << "Unknown block major version: " << block.getBlock().majorVersion << "." << block.getBlock().minorVersion;
  return false;
}

size_t Currency::getApproximateMaximumInputCount(size_t transactionSize, size_t outputCount, size_t mixinCount) const {
  const size_t KEY_IMAGE_SIZE = sizeof(Crypto::KeyImage);
  const size_t OUTPUT_KEY_SIZE = sizeof(decltype(KeyOutput::key));
  const size_t AMOUNT_SIZE = sizeof(uint64_t) + 2; //varint
  const size_t GLOBAL_INDEXES_VECTOR_SIZE_SIZE = sizeof(uint8_t);//varint
  const size_t GLOBAL_INDEXES_INITIAL_VALUE_SIZE = sizeof(uint32_t);//varint
  const size_t GLOBAL_INDEXES_DIFFERENCE_SIZE = sizeof(uint32_t);//varint
  const size_t SIGNATURE_SIZE = sizeof(Crypto::Signature);
  const size_t EXTRA_TAG_SIZE = sizeof(uint8_t);
  const size_t INPUT_TAG_SIZE = sizeof(uint8_t);
  const size_t OUTPUT_TAG_SIZE = sizeof(uint8_t);
  const size_t PUBLIC_KEY_SIZE = sizeof(Crypto::PublicKey);
  const size_t TRANSACTION_VERSION_SIZE = sizeof(uint8_t);
  const size_t TRANSACTION_UNLOCK_TIME_SIZE = sizeof(uint64_t);

  const size_t outputsSize = outputCount * (OUTPUT_TAG_SIZE + OUTPUT_KEY_SIZE + AMOUNT_SIZE);
  const size_t headerSize = TRANSACTION_VERSION_SIZE + TRANSACTION_UNLOCK_TIME_SIZE + EXTRA_TAG_SIZE + PUBLIC_KEY_SIZE;
  const size_t inputSize = INPUT_TAG_SIZE + AMOUNT_SIZE + KEY_IMAGE_SIZE + SIGNATURE_SIZE + GLOBAL_INDEXES_VECTOR_SIZE_SIZE + GLOBAL_INDEXES_INITIAL_VALUE_SIZE +
                            mixinCount * (GLOBAL_INDEXES_DIFFERENCE_SIZE + SIGNATURE_SIZE);

  return (transactionSize - headerSize - outputsSize) / inputSize;
}

Currency::Currency(Currency&& currency) :
m_maxBlockHeight(currency.m_maxBlockHeight),
m_maxBlockBlobSize(currency.m_maxBlockBlobSize),
m_maxTxSize(currency.m_maxTxSize),
m_publicAddressBase58Prefix(currency.m_publicAddressBase58Prefix),
m_minedMoneyUnlockWindow(currency.m_minedMoneyUnlockWindow),
m_expectedNumberOfBlocksPerDay(currency.m_expectedNumberOfBlocksPerDay),
m_timestampCheckWindow(currency.m_timestampCheckWindow),
m_timestampCheckWindowV2(currency.m_timestampCheckWindowV2),
m_blockFutureTimeLimit(currency.m_blockFutureTimeLimit),
m_blockFutureTimeLimitV2(currency.m_blockFutureTimeLimitV2),
m_moneySupply(currency.m_moneySupply),
m_emissionSpeedFactor(currency.m_emissionSpeedFactor),
m_rewardBlocksWindow(currency.m_rewardBlocksWindow),
m_blockGrantedFullRewardZone(currency.m_blockGrantedFullRewardZone),
m_isBlockexplorer(currency.m_isBlockexplorer),
m_minerTxBlobReservedSize(currency.m_minerTxBlobReservedSize),
m_maxTransactionSizeLimit(currency.m_maxTransactionSizeLimit),
m_numberOfDecimalPlaces(currency.m_numberOfDecimalPlaces),
m_coin(currency.m_coin),
m_mininumFee(currency.m_mininumFee),
m_defaultDustThreshold(currency.m_defaultDustThreshold),
m_difficultyTarget(currency.m_difficultyTarget),
m_difficultyWindow(currency.m_difficultyWindow),
m_difficultyLag(currency.m_difficultyLag),
m_difficultyCut(currency.m_difficultyCut),
m_maxBlockSizeInitial(currency.m_maxBlockSizeInitial),
m_maxBlockSizeGrowthSpeedNumerator(currency.m_maxBlockSizeGrowthSpeedNumerator),
m_maxBlockSizeGrowthSpeedDenominator(currency.m_maxBlockSizeGrowthSpeedDenominator),
m_lockedTxAllowedDeltaSeconds(currency.m_lockedTxAllowedDeltaSeconds),
m_lockedTxAllowedDeltaBlocks(currency.m_lockedTxAllowedDeltaBlocks),
m_mempoolTxLiveTime(currency.m_mempoolTxLiveTime),
m_numberOfPeriodsToForgetTxDeletedFromPool(currency.m_numberOfPeriodsToForgetTxDeletedFromPool),
m_fusionTxMaxSize(currency.m_fusionTxMaxSize),
m_fusionTxMinInputCount(currency.m_fusionTxMinInputCount),
m_fusionTxMinInOutCountRatio(currency.m_fusionTxMinInOutCountRatio),
m_upgradeHeightV2(currency.m_upgradeHeightV2),
m_upgradeHeightV3(currency.m_upgradeHeightV3),
m_upgradeVotingThreshold(currency.m_upgradeVotingThreshold),
m_upgradeVotingWindow(currency.m_upgradeVotingWindow),
m_upgradeWindow(currency.m_upgradeWindow),
m_blocksFileName(currency.m_blocksFileName),
m_blockIndexesFileName(currency.m_blockIndexesFileName),
m_txPoolFileName(currency.m_txPoolFileName),
m_cryptonoteCoinVersion(currency.m_cryptonoteCoinVersion),

m_zawyLWMA2DifficultyBlockIndex(currency.m_zawyLWMA2DifficultyBlockIndex),
m_zawyLWMA2DifficultyN(currency.m_zawyLWMA2DifficultyN),

m_governancePercent(currency.m_governancePercent),
m_governanceHeightStart(currency.m_governanceHeightStart),
m_governanceHeightEnd(currency.m_governanceHeightEnd),

m_testnet(currency.m_testnet),
genesisBlockTemplate(std::move(currency.genesisBlockTemplate)),
cachedGenesisBlock(new CachedBlock(genesisBlockTemplate)),
logger(currency.logger) {
}

CurrencyBuilder::CurrencyBuilder(Logging::ILogger& log) : m_currency(log) {
  maxBlockNumber(parameters::CRYPTONOTE_MAX_BLOCK_NUMBER);
  maxBlockBlobSize(parameters::CRYPTONOTE_MAX_BLOCK_BLOB_SIZE);
  maxTxSize(parameters::CRYPTONOTE_MAX_TX_SIZE);
  publicAddressBase58Prefix(parameters::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX);
  minedMoneyUnlockWindow(parameters::CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW);
  expectedNumberOfBlocksPerDay(parameters::EXPECTED_NUMBER_OF_BLOCKS_PER_DAY);

  timestampCheckWindow(parameters::BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW);
  timestampCheckWindowV2(parameters::BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V2);

  blockFutureTimeLimit(parameters::CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT);
  blockFutureTimeLimitV2(parameters::CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V2);

  moneySupply(parameters::MONEY_SUPPLY);
  emissionSpeedFactor(parameters::EMISSION_SPEED_FACTOR);
  cryptonoteCoinVersion(parameters::CRYPTONOTE_COIN_VERSION);

  zawyLWMA2DifficultyBlockIndex(parameters::ZAWY_LWMA2_DIFFICULTY_BLOCK_INDEX);
  zawyLWMA3DifficultyBlockIndex(parameters::ZAWY_LWMA3_DIFFICULTY_BLOCK_INDEX);
  zawyLWMA2DifficultyN(parameters::ZAWY_LWMA2_DIFFICULTY_N);

  governancePercent(parameters::GOVERNANCE_PERCENT);
  governanceHeightStart(parameters::GOVERNANCE_HEIGHT_START);
  governanceHeightEnd(parameters::GOVERNANCE_HEIGHT_END);

  rewardBlocksWindow(parameters::CRYPTONOTE_REWARD_BLOCKS_WINDOW);
  blockGrantedFullRewardZone(parameters::CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE);
  minerTxBlobReservedSize(parameters::CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE);
  maxTransactionSizeLimit(parameters::MAX_TRANSACTION_SIZE_LIMIT);

  numberOfDecimalPlaces(parameters::CRYPTONOTE_DISPLAY_DECIMAL_POINT);

  mininumFee(parameters::MINIMUM_FEE);
  defaultDustThreshold(parameters::DEFAULT_DUST_THRESHOLD);

  difficultyTarget(parameters::DIFFICULTY_TARGET);
  difficultyWindow(parameters::DIFFICULTY_WINDOW);
  difficultyLag(parameters::DIFFICULTY_LAG);
  difficultyCut(parameters::DIFFICULTY_CUT);

  maxBlockSizeInitial(parameters::MAX_BLOCK_SIZE_INITIAL);
  maxBlockSizeGrowthSpeedNumerator(parameters::MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR);
  maxBlockSizeGrowthSpeedDenominator(parameters::MAX_BLOCK_SIZE_GROWTH_SPEED_DENOMINATOR);

  lockedTxAllowedDeltaSeconds(parameters::CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS);
  lockedTxAllowedDeltaBlocks(parameters::CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS);

  mempoolTxLiveTime(parameters::CRYPTONOTE_MEMPOOL_TX_LIVETIME);
  mempoolTxFromAltBlockLiveTime(parameters::CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME);
  numberOfPeriodsToForgetTxDeletedFromPool(parameters::CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL);

  //fusionTxMaxSize(parameters::FUSION_TX_MAX_SIZE);
  // fusion transactions fix
  fusionTxMaxSize(parameters::MAX_TRANSACTION_SIZE_LIMIT * 30 / 100);
  fusionTxMinInputCount(parameters::FUSION_TX_MIN_INPUT_COUNT);
  fusionTxMinInOutCountRatio(parameters::FUSION_TX_MIN_IN_OUT_COUNT_RATIO);

  upgradeHeightV2(parameters::UPGRADE_HEIGHT_V2);
  upgradeHeightV3(parameters::UPGRADE_HEIGHT_V3);
  upgradeVotingThreshold(parameters::UPGRADE_VOTING_THRESHOLD);
  upgradeVotingWindow(parameters::UPGRADE_VOTING_WINDOW);
  upgradeWindow(parameters::UPGRADE_WINDOW);

  blocksFileName(parameters::CRYPTONOTE_BLOCKS_FILENAME);
  blockIndexesFileName(parameters::CRYPTONOTE_BLOCKINDEXES_FILENAME);
  txPoolFileName(parameters::CRYPTONOTE_POOLDATA_FILENAME);

    isBlockexplorer(false);
  testnet(false);
}

Transaction CurrencyBuilder::generateGenesisTransaction() {
  CryptoNote::Transaction tx;
  CryptoNote::AccountPublicAddress ac = boost::value_initialized<CryptoNote::AccountPublicAddress>();
  m_currency.constructMinerTx(1, 0, 0, 0, 0, 0, ac, tx); // zero fee in genesis
  return tx;
}
CurrencyBuilder& CurrencyBuilder::emissionSpeedFactor(unsigned int val) {
  if (val <= 0 || val > 8 * sizeof(uint64_t)) {
    throw std::invalid_argument("val at emissionSpeedFactor()");
  }

  m_currency.m_emissionSpeedFactor = val;
  return *this;
}

CurrencyBuilder& CurrencyBuilder::numberOfDecimalPlaces(size_t val) {
  m_currency.m_numberOfDecimalPlaces = val;
  m_currency.m_coin = 1;
  for (size_t i = 0; i < m_currency.m_numberOfDecimalPlaces; ++i) {
    m_currency.m_coin *= 10;
  }

  return *this;
}

CurrencyBuilder& CurrencyBuilder::difficultyWindow(size_t val) {
  if (val < 2) {
    throw std::invalid_argument("val at difficultyWindow()");
  }
  m_currency.m_difficultyWindow = val;
  return *this;
}

CurrencyBuilder& CurrencyBuilder::upgradeVotingThreshold(unsigned int val) {
  if (val <= 0 || val > 100) {
    throw std::invalid_argument("val at upgradeVotingThreshold()");
  }

  m_currency.m_upgradeVotingThreshold = val;
  return *this;
}

CurrencyBuilder& CurrencyBuilder::upgradeWindow(uint32_t val) {
  if (val <= 0) {
    throw std::invalid_argument("val at upgradeWindow()");
  }

  m_currency.m_upgradeWindow = val;
  return *this;
}

}
