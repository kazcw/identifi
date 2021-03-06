#ifndef IDENTIFI_DATA_H
#define IDENTIFI_DATA_H

#include <string>
#include <vector>
#include <boost/regex.hpp>
#include "hash.h"
#include "util.h"
#include "key.h"
#include "base58.h"
#include "serialize.h"

#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_utils.h"

using namespace std;
using namespace boost;

typedef std::pair<string, string> string_pair;

struct IdentifiKey {
    string pubKey;
    string keyID;
    string privKey;
};

IdentifiKey CKeyToIdentifiKey(CKey& key);

struct LinkedID {
    string_pair id;
    int confirmations;
    int refutations;
};

struct IDOverview {
    int receivedPositive;
    int receivedNeutral;
    int receivedNegative;
    int authoredPositive;
    int authoredNeutral;
    int authoredNegative;
    long int firstSeen;
};

class CSignature {
public:
    CSignature(string signerPubKey = "", string signature = "", string signerKeyID = "") : signerPubKey(signerPubKey), signature(signature), signerKeyID(signerKeyID) {}
    string GetSignerPubKey() const;
    string GetSignature() const;
    string GetSignerKeyID();
    bool IsValid(string signedData) const;
    json_spirit::Object GetJSON();

private:
    string signerPubKey;
    string signature;
    string signerKeyID;         
};

class CIdentifiPacket {
public:
    CIdentifiPacket(string strData = "", bool skipVerify = false) {
        if (!strData.empty())
            SetData(strData, skipVerify);
        published = false;
    }
    bool operator== (const CIdentifiPacket &r) const {
        return (r.GetHash() == GetHash() && r.timestamp == timestamp);
    }
    bool operator!= (const CIdentifiPacket &r) const {
        return (r.GetHash() != GetHash() || r.timestamp != timestamp);
    }
    void SetData(string strData, bool skipVerify = false);
    void SetPublished();
    bool IsPublished();
    int GetPriority();
    void SetPriority(int priority);
    bool Sign(CKey& key);
    bool AddSignature(CSignature signature);
    int GetRating() const;
    int GetMinRating() const;
    int GetMaxRating() const;
    string GetComment() const;
    string GetType() const;
    string GetData() const;
    string GetSignedData() const;
    uint256 GetSignedDataHash() const;
    uint256 GetHash() const;
    time_t GetTimestamp() const;
    vector<string_pair > GetAuthors() const;
    vector<string_pair > GetRecipients() const;
    CSignature GetSignature() const;
    json_spirit::Value GetJSON();

    IMPLEMENT_SERIALIZE
    (
        string strData;
        if (fWrite) {
            strData = GetData();
            READWRITE(strData);
        } else {
            CIdentifiPacket *packet = const_cast<CIdentifiPacket*>(this);
            READWRITE(strData);
            packet->SetData(strData);
            packet->SetPublished();
        }
    )

private:
    string strData;
    string comment;
    string type;
    int rating;
    int maxRating;
    int minRating;
    vector<string_pair > authors;
    vector<string_pair > recipients;
    CSignature signature;
    time_t timestamp;
    bool published;
    int priority;
    void UpdateSignatures();
};

#endif // IDENTIFI_DATA_H