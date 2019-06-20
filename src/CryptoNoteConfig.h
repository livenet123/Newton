// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
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

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>

namespace CryptoNote {
namespace parameters {

const uint64_t DIFFICULTY_TARGET                             = 120; // seconds

const uint32_t CRYPTONOTE_MAX_BLOCK_NUMBER                   = 500000000;
const size_t   CRYPTONOTE_MAX_BLOCK_BLOB_SIZE                = 500000000;
const size_t   CRYPTONOTE_MAX_TX_SIZE                        = 1000000000;
const uint64_t CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX       = 0x7b54;  //addresses start with "cc"
const uint32_t CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW          = 60;
const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT            = 60 * 60 * 2;
const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V2         = 3 * DIFFICULTY_TARGET;

const size_t   BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW             = 60;
const uint32_t BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V2          = 11;

// MONEY_SUPPLY - total number coins to be generated
const uint64_t MONEY_SUPPLY                                  = ((uint64_t)(-1)); // 184 B
const size_t   CRYPTONOTE_COIN_VERSION                       = 1;

const uint32_t ZAWY_LWMA2_DIFFICULTY_BLOCK_INDEX             = 145000;
const uint32_t ZAWY_LWMA3_DIFFICULTY_BLOCK_INDEX             = 941000;
const size_t   DIFFICULTY_WINDOW_V2                          = 60;
const size_t   ZAWY_LWMA2_DIFFICULTY_N                 		   = DIFFICULTY_WINDOW_V2;
const uint64_t DIFFICULTY_BLOCKS_COUNT_V2                    = DIFFICULTY_WINDOW_V2 + 1;

const uint16_t GOVERNANCE_PERCENT                             = 20;  // 20 percent of block reward
const uint32_t GOVERNANCE_HEIGHT_START                        = 196000;
const uint32_t GOVERNANCE_HEIGHT_END                          = 941000;

const unsigned EMISSION_SPEED_FACTOR                         = 18;
static_assert(EMISSION_SPEED_FACTOR <= 8 * sizeof(uint64_t), "Bad EMISSION_SPEED_FACTOR");

const size_t   CRYPTONOTE_REWARD_BLOCKS_WINDOW               = 100;
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE     = 350000; //size of block (bytes) after which reward for block calculated using block size
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2  = 350000;
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1  = 20000;
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_CURRENT = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE;
const size_t   CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE        = 600;
const size_t   CRYPTONOTE_DISPLAY_DECIMAL_POINT              = 8;
const uint64_t MINIMUM_FEE                                   = 100000;
const uint64_t DEFAULT_DUST_THRESHOLD                        = MINIMUM_FEE;
const uint64_t MAX_TRANSACTION_SIZE_LIMIT                    = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE * 110 / 100 - CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE;


const uint64_t EXPECTED_NUMBER_OF_BLOCKS_PER_DAY             = 24 * 60 * 60 / DIFFICULTY_TARGET;
const size_t   DIFFICULTY_WINDOW                             = 17;
const size_t   DIFFICULTY_WINDOW_V1                          = 720;
const size_t   DIFFICULTY_CUT                                = 0;  // timestamps to cut after sorting
const size_t   DIFFICULTY_CUT_V1                             = 60;
const size_t   DIFFICULTY_CUT_V2                             = 60;
const size_t   DIFFICULTY_LAG                                = 0;  // !!!
const size_t   DIFFICULTY_LAG_V1                             = 15;
const size_t   DIFFICULTY_LAG_V2                             = 15;
static_assert(2 * DIFFICULTY_CUT <= DIFFICULTY_WINDOW - 2, "Bad DIFFICULTY_WINDOW or DIFFICULTY_CUT");

const uint64_t POISSON_CHECK_TRIGGER						 = 10; // Reorg size that triggers poisson timestamp check
const uint64_t POISSON_CHECK_DEPTH							 = 60;   // Main-chain depth of the poisson check. The attacker will have to tamper 50% of those blocks
const double   POISSON_LOG_P_REJECT							 = -75.0; // Reject reorg if the probablity that the timestamps are genuine is below e^x, -75 = 10^-33

const size_t   MAX_BLOCK_SIZE_INITIAL                        = 500 * 1024;
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR         = 100 * 1024;
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_DENOMINATOR       = 365 * 24 * 60 * 60 / DIFFICULTY_TARGET;

const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS     = 1;
const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS    = DIFFICULTY_TARGET * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS;

const uint64_t CRYPTONOTE_MEMPOOL_TX_LIVETIME                = 60 * 60 * 24;     //seconds, one day
const uint64_t CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME = 60 * 60 * 24 * 7; //seconds, one week
const uint64_t CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL = 7;  // CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL * CRYPTONOTE_MEMPOOL_TX_LIVETIME = time to forget tx

const size_t   FUSION_TX_MAX_SIZE                            = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_CURRENT * 30 / 100;
const size_t   FUSION_TX_MIN_INPUT_COUNT                     = 12;
const size_t   FUSION_TX_MIN_IN_OUT_COUNT_RATIO              = 4;

const uint32_t KEY_IMAGE_CHECKING_BLOCK_INDEX                = 0;
const uint32_t UPGRADE_HEIGHT_V2                             = 145000;
const uint32_t UPGRADE_HEIGHT_V3                             = 4294967294;
const unsigned UPGRADE_VOTING_THRESHOLD                      = 90;               // percent
const uint32_t UPGRADE_VOTING_WINDOW                         = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY;  // blocks
const uint32_t UPGRADE_WINDOW                                = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY;  // blocks
static_assert(0 < UPGRADE_VOTING_THRESHOLD && UPGRADE_VOTING_THRESHOLD <= 100, "Bad UPGRADE_VOTING_THRESHOLD");
static_assert(UPGRADE_VOTING_WINDOW > 1, "Bad UPGRADE_VOTING_WINDOW");

const char     CRYPTONOTE_BLOCKS_FILENAME[]                  = "blocks.bin";
const char     CRYPTONOTE_BLOCKINDEXES_FILENAME[]            = "blockindexes.bin";
const char     CRYPTONOTE_POOLDATA_FILENAME[]                = "poolstate.bin";
const char     P2P_NET_DATA_FILENAME[]                       = "p2pstate.bin";
const char     MINER_CONFIG_FILE_NAME[]                      = "miner_conf.json";
} // parameters

const char     CRYPTONOTE_NAME[]                             = "Newton";

const uint8_t  TRANSACTION_VERSION_1                         =  1;
const uint8_t  TRANSACTION_VERSION_2                         =  2;
const uint8_t  CURRENT_TRANSACTION_VERSION                   =  TRANSACTION_VERSION_1;
const uint8_t  BLOCK_MAJOR_VERSION_1                         =  1;
const uint8_t  BLOCK_MAJOR_VERSION_2                         =  2;
const uint8_t  BLOCK_MAJOR_VERSION_3                         =  3;
const uint8_t  BLOCK_MINOR_VERSION_0                         =  0;
const uint8_t  BLOCK_MINOR_VERSION_1                         =  1;

const size_t   BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT        =  10000;  //by default, blocks ids count in synchronizing
const size_t   BLOCKS_SYNCHRONIZING_DEFAULT_COUNT            =  128;    //by default, blocks count in blocks downloading
const size_t   COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT         =  1000;

const int      P2P_DEFAULT_PORT								               = 30236;
const int      RPC_DEFAULT_PORT                              = 31236;

const size_t   P2P_LOCAL_WHITE_PEERLIST_LIMIT                =  1000;
const size_t   P2P_LOCAL_GRAY_PEERLIST_LIMIT                 =  5000;

const size_t   P2P_CONNECTION_MAX_WRITE_BUFFER_SIZE          = 64 * 1024 * 1024; // 64 MB
const uint32_t P2P_DEFAULT_CONNECTIONS_COUNT                 = 8;
const size_t   P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT     = 70;
const uint32_t P2P_DEFAULT_HANDSHAKE_INTERVAL                = 60;            // seconds
const uint32_t P2P_DEFAULT_PACKET_MAX_SIZE                   = 50000000;      // 50000000 bytes maximum packet size
const uint32_t P2P_DEFAULT_PEERS_IN_HANDSHAKE                = 250;
const uint32_t P2P_DEFAULT_CONNECTION_TIMEOUT                = 5000;          // 5 seconds
const uint32_t P2P_DEFAULT_PING_CONNECTION_TIMEOUT           = 2000;          // 2 seconds
const uint64_t P2P_DEFAULT_INVOKE_TIMEOUT                    = 60 * 2 * 1000; // 2 minutes
const size_t   P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT          = 5000;          // 5 seconds
const char     P2P_STAT_TRUSTED_PUB_KEY[]                    = "";

const char* const SEED_NODES[] = {

  "185.198.58.13:30236",
  "185.198.58.31:30236",
	"185.45.192.241:30236",
	"185.45.192.15:30236"

 };


std::string const GOVERNANCE_WALLET_ADDRESS                   = "cczJoyYFvf5gBPndTdZHsQSu4MRU4hsg2h7gr4hZXrqpa3YLy1aeMixh2rQtUTQkib6uRUyNaYpya4yHiBm81AKe3JMPeosA7p";
std::string const GOVERNANCE_VIEW_SECRET_KEY                  = "ec0dcadca1936177c4cd7b98629cc80d49f4deef2ef046a8f2b047271d229704";

std::string const TESTNET_GOVERNANCE_WALLET_ADDRESS           = "cczJijPPtCa6K2kQ2LxGpU9kU1oBCgmoaCE6o4naV64p3YsehAeF2MqbPts7FedfLJEPnW49HdDUq9reRy6bmb4u9qmJWXRMtT";
std::string const TESTNET_GOVERNANCE_VIEW_SECRET_KEY          = "7626ee1dcf5492754b8a90c8bb3dcb42f4b71ab0e5d7478b72baa713a1e59706";


struct CheckpointData {
  uint32_t index;
  const char* blockId;
};

const std::initializer_list<CheckpointData> CHECKPOINTS = {

	{0, "d5944f7fba38cb24141793acde93751c7b71fef0647b84e2058bafeeac707bb9"},
	{4000, "ea9d210add8c7148765a8308b15f6b4e81640f951e4474625044370627ffcb62"},
	{8000, "1b1dba42a97e164cfb45c31705b233e9279f0ad0a6fbad26166007f4920c580d"},
	{12000, "e61645bc7e7aaf6b5429d1b67c6f712ec8c132d8251bd48fc6d0d3137c54bf5f"},
	{16000, "b07be4c5d236eb7f995db47c7f5aac818d7087b6eb81ccd9107835c47c8cdc3c"},
	{20000, "420a828c2c53a074adc435860aae3132ac63ef986f826a784aac7725dc624a3d"},
	{24000, "0bcf548ed7a9b332ce11ffd72e46e4d2f9216b37b9d0555a1670bcfed3c5d5f4"},
	{28000, "46978e971207cb8a6af517e87c34cf52bda6c2ca6b2f87bb51009fbd215596fb"},
	{32000, "6040e15c31238292dbc4d92cd4cf7abe43303ada2de3bb78f671b4b270c02c70"},
	{36000, "43e6d399d5c58e7c9c5a5b21f7b9f7d7a70a6333dd46a195279e0967a1d7dfa1"},
	{40000, "ed48feb77d135e1669bf70abca52edd94f8a0b9f69ae25781c4782cca007e74e"},
	{44000, "af6e3e92473bb8197321a6e12b2599207333beec14d2bf918bac1f7eab257e6d"},
	{48000, "17d2896b85205433de236e3f0ad5f32f99b762606e49130709cd22ddd43634c1"},
	{52000, "c78fe43026fa9d14e3b1be50cfc0561aa49367d2e291969361bc56bbecf73000"},
	{56000, "c261e031cfc9bcede0a339cbb973d67519fd6810b1416c011c75de71e13e01d9"},
	{60000, "850372abd1c71f0424e1b0e11d605963fc535c953405911ac28a7d0546ccc7c6"},
	{64000, "905d5839524891087e3a1d66777b06f73f3068137db4b95beed8a8399b001775"},
	{68000, "9d134bd70bb5826d78824e03fc86727c33cf96e4a8643d8e8668da5561e775e5"},
	{72000, "04496c511860b3a521dcaa874b840a54cc3700f24c760923e63f0c6348fc9d09"},
	{76000, "39efeba3a8bc53d71ef3c980603d4f8aa4ddaabad3a28d24eb423c8c1694072f"},
	{80000, "d2c69829de73a96a5b35c2c693f071631caf076444d80f56a07ebef4e435880b"},
	{84000, "222056359ae4fe8f0466a68f81f056e4ef6d3cd5b1a31ff4ad131ce269011702"},
	{88000, "71440754ceb0838d9c08e13536c00b8271820eeacca7988ef89073ad059f78b6"},
	{92000, "3d74fb2ffabe3663bf7b2df3bf98119369412c263981d45015e719906958c920"},
	{96000, "47609820e6ee77b03d0044500355c8737ec285cd79fa71e9ca060246f8af121e"},
	{100000, "525e37cc41eaf62dd120772ac5eba93b56c884cfa04b07c097c2cb386f0036f7"},
	{104000, "fa7ab517ecfb357163fa441a3124ebb8b6adf1ca2b589b2ff652d329f16a05a8"},
	{108000, "231427520968dc83688918da33fff4558961f4bfcd1ce0537de7662b9fddff9f"},
	{112000, "d6d236f71f486ad09f0712435341affa37cbf2376a0e140a64df4baec5a905d4"},
	{116000, "fd904d5c4c9b7217c9aef8e7a46a450d326676a847024be533fc55524d46ac2c"},
	{120000, "7ce836e22e53d2a35816e42a98e1322af30e33620bf7d469b3f0bc0fb0ac98b3"},
	{124000, "1a038acc443317a58933d5ececa974486edf4936ef7cf52d27797e023fe6a281"},
	{128000, "7c33bd046eaa95da1ed89faa1f98ab961963755ca77c72a1682d82c5185afbb9"},
	{132000, "b5bc1b8c496bcf4da14a496f2430a2338d39ff9e4ea6a2bd85b00624bd976bf6"},
	{136000, "e20bf930e52a239c2333339ba92f3ecd7940e79ced5631d035a36792acadeb78"},
	{140000, "6915889a9a52c12b2f43a97a7fbb278678058ec439b7084a2d4de9d13001a7ea"},
	{144000, "933bf4fcc6d195b19948cac600b9f665caea1d280c33c01ee9353e20e1b10bae"},
	{148000, "75e3bfdf46c7bc1287b4de3ea2146c5991c2cffb965778ea9cf2fff2a9bceaf1"},
	{152000, "a78decfcf3fff41b927655b9cfbe640b7c599e37e73b5fa41b0ce2fbe6e6eb59"},
	{156000, "b399bf53df460b3c3aeae7bf830ed95ed21da2dfb8046f69cdec386340206f73"},
	{160000, "56b9a3edf1adb02cceca3307eaa1694fd3d22b54e8b25e5567e28f9791d9a1ca"},
	{164000, "bb1567fdac2b9b05884566cfb7911d604f1c48e782c5eba4e666eaf3e4ab5b8b"},
	{168000, "70655b33c799369557d1a63e614e67896396ab709d85f4cf7473b3e6a6d2f7d8"},
	{172000, "02172bb159f7760b2aae1698a730bfb01525b2e952d96c3e2223966869a41e66"},
	{176000, "141c4ab276c7fc66a6d4fd79c9cf8948b2f9f4b44719b5da2b04b70dcfb1ed8f"},
	{180000, "29589d17e10de9488c44f6a87b7965c32d85f7c250c4e02fb211480dd9b3bff0"},
	{184000, "53fb8da65f4e8b9591382cd81d3034a3e755b878a8f21a3d4c7ee5f05665e2df"},
  {188000, "321fe8c8f44b53dc9ebefe8c5cfebd6cec96f3c6d87c47f78f9dd581d2232ef6"},
  {192000, "1fdbab5e54988c4a52543b59d0fd97794c40b761408837521c40727b13fa0067"},
  {196000, "f0646c7f4c7a62dc0abc3df3ec2b10501685058aade348f3d3f03e53e71a1761"},
  {200000, "0ab8beff6dd33585dab367aca4012b79e6222399e260a803a828febac7b0f452"},
  {204000, "7a23a71036262f3e0a227f0c1ca08beadfb5833b17045b850948ac7e505b7885"},
  {208000, "548e608371fcc1ae8a0fe8e9f3854f69e02f13f568c44169c82eee30cbe0929c"},
	{212000, "bd7593f657edeb1a6ff1cefedb3ffecab82c5d3e6cb50d1cb1cd5b508032c721"},
	{216000, "ffec1a59e354c430ec43ac7e179a0e887add184b96d47cf734de028f2573878d"},
	{220000, "f594c1642531430b91391d0695c09252ecc25c499378070a2127e10ea72f8820"},
	{224000, "075b31a89f9c4e8f2ef03522e8a52d4803685e01d2afc08919d179fc40c45ab8"},
	{228000, "d16008f616ccd035b630294f3bd75ac92f76cddada86925a01bb9e9044d0124b"},
	{232000, "4a792ca56385cbaf6d5c87eefbb47781d4fa0fb2b28b1e9a1faae3f7b4fd8a72"},
	{236000, "35cefc162199de285efe8f532e7d3a131847ba528726f8b63d4fffd6f7d7cc61"},
	{240000, "9008ef78f122f79cee5214f525dcd09e1999a283018033eaec33ff47d8d7553c"},
	{244000, "3006c8dee29ef6df2bc3a6cabef428321bc55cbee2570e488f94c1ec38cad240"},
	{248000, "8fe4b2bdbca58f942c18bb4cd279db494ff6fad1cf54bd072edcedc57ccb7d91"},
	{252000, "0c5d560095a69e55eefc8d2d16b2c75561cee71cdeda353645c4e3d11ccbc991"},
	{256000, "e5c6e5ff16883996b9fdff7928ad4e9b9e9d58adce5530eef29181d39e966ec1"},
	{260000, "7f6cef4389adebb496defe66e9eee509fbdfdcb8489240e35fc4169a0fee70ff"},
	{264000, "32e56053a97e25ebf939ddfeb78c22e039b0b03433aa5888873c6dbd3ed6ed56"},
	{268000, "2124d9918d5336a654b2392a6d31e738a5dba39100b90f509bcadd1a1fee8c90"},
	{272000, "8a4cd7ba454a175325bdea0abfff81bb61be230319f531d26b42799a8bba942b"},
	{276000, "60df2c6dd69583243f0e9fc37405e17b389e2114963017e88e9a98d47fa05f8f"},
	{280000, "56848aba4061f8b17ba752617ca14fe6ed0e4d440132e3a94f469ad91d7c6f5c"},
	{284000, "e1e315f79f3e50dad4565bf1b9e7b8ecb0fa42b043fe1de6e48569ce363fb209"},
	{288000, "7cce7f93442e2ee447d1c4398776e745e8eae12dcfdfb96f4df13823227e9880"},
	{292000, "0ddb52388234002246c6ab45675268ba7c4f0955bf6f6e2a2406f4a25e3fc187"},
	{296000, "5ef57607f95f185da7cb695eac6f253995704e1805e6c6588fa9745f1066ce1d"},
	{300000, "7c13fe58ee1e862f28a8e127670b7b834c2689e4dbd798efad4a9d05f9f1cdee"},
	{375000, "4b193fd43754355e27ea232309ec1566da96d793ff91e110d6cce7dff064fdf4"},
	{450000, "d268b8467a429ba3ef9f65cedfe0d73fbc29b68efb57a9cabd45845a2e81c823"},
	{525000, "3039d0a1b63d284588d38ac55f303f553791090815cae89b653e316b6bb6ae98"},
	{600000, "9c5a0f6e78c18c07d02ee9055dac394a95a06bac45399c036b61a0f5a7e45010"},
	{675000, "4b8e2475e49b32871e7fe22356dc2588be1be23cddb23178139c1e28fbfe3578"},
	{750000, "c387364a1b0e85a4f494568e140af5627fe4cd17bfd60d15f9b17abdce358547"},
	{825000, "6e037e44a69be0cf57f9600861ef1cb5119b898b079a2865c222727c2bb427d3"},
	{900000, "ec5467538185b89ba2c6334ec1dffdd655f18120aad727bc0aaa7833b1177106"},
	{934145, "4a238d5bb901f7362eaa422743e31623b96c3c8bd85bb8c95ad5f858f8950588"}

 };

} // CryptoNote

#define ALLOW_DEBUG_COMMANDS
