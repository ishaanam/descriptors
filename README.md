# Output Script Descriptor Parser
This parser currently only supports the pk(), pkh(), wpkh(), sh(), wsh(), and multi() script expressions. This parser doesn't really do anything useful with the descriptor after it parses it, I mainly made it to learn about descriptors and to potentially build off of it in the future.

### Reference BIPs
+ [BIP 380: Output Script Descriptors General Operation](https://github.com/bitcoin/bips/blob/master/bip-0380.mediawiki)
+ [BIP 381: Non-Segwit Output Script Descriptors: pk(), pkh(), sh()](https://github.com/bitcoin/bips/blob/master/bip-0381.mediawiki)
+ [BIP 382: Segwit Output Script Descriptors: wpkh(), wsh()](https://github.com/bitcoin/bips/blob/master/bip-0382.mediawiki)
+ [BIP 383: Multisig Output Script Descriptors: multi(), sortedmulti()](https://github.com/bitcoin/bips/blob/master/bip-0383.mediawiki)
