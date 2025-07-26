#pragma once
#define CURRENT_SERVER_VERSION 1
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <array>
#include <cstdint>
#include <cstring>
enum PacketType : uint8_t
{
	SYNC_CLIENT_INFO = 0,
	CONNECT = 1,
	SYNC_USERS = 2,
	DISCONNECT = 3,
	ID = 4,
	MOVE = 5,
	ROTATE = 6,
	ANIMATE = 7,
	DAMAGE = 8,
	STAGE_END = 9,
	CHAT = 10,
	CHAT_STARTED = 11,
	STAGE_START = 12,
	VERSION_CHECK = 13,
	RACE_UPDATE = 14,
	ANIM_SPEED = 15,
	ANIM_BLEND = 16,
	KICK_REASON = 17,
	COLOR = 18,
	STEAM_ID = 19
};
enum ServerInfoRequestType : uint8_t
{
	NOT_SENT,
	APPROVED,
	DENIED_WRONG_MODS,
	DENIED_TOO_MANY_PLAYERS,
	DENIED_WRONG_VERSION
};
struct SimpleVec4
{
public:
	float x, y, z, w;
	SimpleVec4(float inx, float iny, float inz, float inw)
	{
		x = inx;
		y = iny;
		z = inz;
		w = inw;
	}
	SimpleVec4(float inx, float iny, float inz)
	{
		x = inx;
		y = iny;
		z = inz;
		w = 1;
	}
	SimpleVec4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	};
	std::array<float, 4> data()
	{
		return { x, y, z, w };
	}
	SimpleVec4 operator+(const SimpleVec4& c2)const
	{
		SimpleVec4 ret = SimpleVec4();
		ret.x = x + c2.x;
		ret.y = y + c2.y;
		ret.z = z + c2.z;
		ret.w = w + c2.w;
		return ret;
	}
	SimpleVec4 operator-(SimpleVec4 c2)const 
	{
		auto ret = SimpleVec4(x, y, z, w);
		ret.x -= c2.x;
		ret.y -= c2.y;
		ret.z -= c2.z;
		ret.w -= c2.w;
		return ret;
	}
	SimpleVec4 operator*(SimpleVec4 c2) const
	{
		auto ret = SimpleVec4(x, y, z, w);
		ret.x *= c2.x;
		ret.y *= c2.y;
		ret.z *= c2.z;
		ret.w *= c2.w;
		return ret;
	}
	SimpleVec4 operator*(int c2) const
	{
		auto ret = SimpleVec4(x, y, z, w);
		ret.x *= c2;
		ret.y *= c2;
		ret.z *= c2;
		ret.w *= c2;
		return ret;
	}
	SimpleVec4 operator/(SimpleVec4 c2) const
	{
		auto ret = SimpleVec4(x, y, z, w);
		ret.x /= c2.x;
		ret.y /= c2.y;
		ret.z /= c2.z;
		ret.w /= c2.w;
		return ret;
	}
#if BBM_CLIENT
	operator hh::math::CVector4() const { return hh::math::CVector4(x, y, z, w); }
	operator hh::math::CVector() const { return hh::math::CVector(x, y, z); }
#endif
};
#define SERIALIZE_BASE_PACKET() memcpy(buffer + offset, &type, sizeof(type)); \
offset += sizeof(type); \
memcpy(buffer + offset, &senderId, sizeof(senderId)); \
offset += sizeof(senderId);

#define DESERIALIZE_BASE_PACKET() PacketCommon::DeserializeGeneric(buffer, offset, packet.type); \
PacketCommon::DeserializeGeneric(buffer, offset, packet.senderId);
struct SClientCommonData
{
	int m_id;
	int64_t  m_Steamid;
	std::string m_username;
	std::string m_Stage;
	uint8_t m_iPlayerType;
	SimpleVec4 position;
	SimpleVec4 rotation;
	SimpleVec4 m_vColor;
	float stageRatio;
	SClientCommonData()
	{
		m_id = -1;
		m_Steamid = -1;
		m_username = "";
		m_Stage = "";
		m_iPlayerType = 0;
		position = SimpleVec4(0, 0, 0, 0);
		rotation = SimpleVec4(0, 0, 0, 0);
		m_vColor = SimpleVec4(0, 0, 0, 0);
	}
};
class PacketCommon
{
public:
	static void SerializeStdString(uint8_t* out_Buff, size_t& out_Offset, const std::string& in_String)
	{
		memcpy(out_Buff + out_Offset, in_String.c_str(), in_String.size() + 1); // include null terminator
		out_Offset += in_String.size() + 1;
	}
	static void DeserializeStdString(const uint8_t* out_Buff, size_t& out_Offset, std::string& in_String, size_t in_Size)
	{
		//Read string
		if (out_Offset >= in_Size) throw std::runtime_error("Invalid Packet Data");
		const char* strStart = reinterpret_cast<const char*>(out_Buff + out_Offset);
		size_t remaining = in_Size - out_Offset;

		//Check if null terminator is within bounds of string (very important!!)
		const char* nullPos = reinterpret_cast<const char*>(memchr(strStart, '\0', remaining));
		if (!nullPos) throw std::runtime_error("No null terminator");

		in_String = std::string(strStart);
	}
	template <typename T>
	static void DeserializeGeneric(const uint8_t* buffer, size_t& offset, T& outValue)
	{
		memcpy(&outValue, buffer + offset, sizeof(T));
		offset += sizeof(T);
	}
	template <typename T>
	static void SerializeGeneric(uint8_t* buffer, size_t& offset, T outValue)
	{
		memcpy(buffer + offset, &outValue, sizeof(outValue));
		offset += sizeof(outValue);
	}
	template <typename T>
	static void SerializeVector(const std::vector<T>& vec, uint8_t* buffer, size_t& offset)
	{
		uint32_t count = static_cast<uint32_t>(vec.size());
		memcpy(buffer + offset, &count, sizeof(count));
		offset += sizeof(count);

		memcpy(buffer + offset, vec.data(), sizeof(T) * vec.size());
		offset += sizeof(T) * vec.size();
	}

	template <typename T>
	static void DeserializeVector(std::vector<T>& vec, const uint8_t* buffer, size_t& offset)
	{
		uint32_t count;
		memcpy(&count, buffer + offset, sizeof(count));
		offset += sizeof(count);

		vec.resize(count);
		memcpy(vec.data(), buffer + offset, sizeof(T) * count);
		offset += sizeof(T) * count;
	}
	static void SerializeStringVector(const std::vector<std::string>& vec, uint8_t* buffer, size_t& offset)
	{
		uint32_t count = static_cast<uint32_t>(vec.size());
		memcpy(buffer + offset, &count, sizeof(count));
		offset += sizeof(count);

		for (const auto& str : vec) {
			uint32_t len = static_cast<uint32_t>(str.size());
			memcpy(buffer + offset, &len, sizeof(len));
			offset += sizeof(len);
			memcpy(buffer + offset, str.data(), len);
			offset += len;
		}
	}

	static void SerializeClientVector(const std::vector<SClientCommonData>& clients, uint8_t* buffer, size_t& offset)
	{
		uint32_t count = static_cast<uint32_t>(clients.size());
		PacketCommon::SerializeGeneric(buffer, offset, count);

		for (const auto& c : clients)
		{
			PacketCommon::SerializeGeneric(buffer, offset, c.m_id);
			PacketCommon::SerializeGeneric(buffer, offset, c.m_Steamid);
			uint32_t lenUser = static_cast<uint32_t>(c.m_username.size());
			PacketCommon::SerializeGeneric(buffer, offset, lenUser);
			memcpy(buffer + offset, c.m_username.data(), lenUser);
			offset += lenUser;

			uint32_t lenStage = static_cast<uint32_t>(c.m_Stage.size());
			PacketCommon::SerializeGeneric(buffer, offset, lenStage);
			memcpy(buffer + offset, c.m_Stage.data(), lenStage);
			offset += lenStage;

			PacketCommon::SerializeGeneric(buffer, offset, c.m_iPlayerType);
			PacketCommon::SerializeGeneric(buffer, offset, c.position);
			PacketCommon::SerializeGeneric(buffer, offset, c.rotation);
			PacketCommon::SerializeGeneric(buffer, offset, c.m_vColor);
		}
	}

	static void DeserializeClientVector(std::vector<SClientCommonData>& clients, const uint8_t* buffer, size_t& offset)
	{
		uint32_t count;
		PacketCommon::DeserializeGeneric(buffer, offset, count);

		clients.reserve(count);

		for (uint32_t i = 0; i < count; ++i)
		{
			SClientCommonData c;

			PacketCommon::DeserializeGeneric(buffer, offset, c.m_id);
			PacketCommon::DeserializeGeneric(buffer, offset, c.m_Steamid);

			uint32_t lenUser;
			PacketCommon::DeserializeGeneric(buffer, offset, lenUser);
			c.m_username = std::string(reinterpret_cast<const char*>(buffer + offset), lenUser);
			offset += lenUser;

			uint32_t lenStage;
			PacketCommon::DeserializeGeneric(buffer, offset, lenStage);
			c.m_Stage = std::string(reinterpret_cast<const char*>(buffer + offset), lenStage);
			offset += lenStage;

			PacketCommon::DeserializeGeneric(buffer, offset, c.m_iPlayerType);
			PacketCommon::DeserializeGeneric(buffer, offset, c.position);
			PacketCommon::DeserializeGeneric(buffer, offset, c.rotation);
			PacketCommon::DeserializeGeneric(buffer, offset, c.m_vColor);

			clients.push_back(std::move(c));
		}
	}

	static void DeserializeStringVector(std::vector<std::string>& vec, const uint8_t* buffer, size_t& offset, size_t totalSize)
	{
		uint32_t count;
		memcpy(&count, buffer + offset, sizeof(count));
		offset += sizeof(count);

		vec.resize(count);
		for (uint32_t i = 0; i < count; ++i) {
			uint32_t len;
			memcpy(&len, buffer + offset, sizeof(len));
			offset += sizeof(len);

			if (offset + len > totalSize)
				throw std::runtime_error("Invalid string length in packet");

			vec[i] = std::string(reinterpret_cast<const char*>(buffer + offset), len);
			offset += len;
		}
	}
	template <typename T>
	static size_t GetVectorSize(const std::vector<T>& vec)
	{
		return sizeof(uint32_t) + sizeof(T) * vec.size();
	}

	static inline size_t GetStringVectorSize(const std::vector<std::string>& vec)
	{
		size_t size = sizeof(uint32_t); // for count
		for (const auto& str : vec) {
			size += sizeof(uint32_t);   // for length prefix
			size += str.size();         // for string data (no null terminator)
		}
		return size;
	}
	static inline size_t GetClientCommonDataVectorSize(const std::vector<SClientCommonData>& vec)
	{
		size_t size = sizeof(uint32_t); // element count

		for (const auto& item : vec)
		{
			size += sizeof(int); //m_id
			size += sizeof(int64_t); //m_id
			size += sizeof(uint32_t); //m_username length
			size += item.m_username.size(); //actual bytes
			size += sizeof(uint32_t); //m_Stage length
			size += item.m_Stage.size(); //actual bytes
			size += sizeof(uint8_t); // m_iPlayerType
			size += sizeof(float) * 4; //position
			size += sizeof(float) * 4; //rotation
			size += sizeof(float) * 4; //color
		}

		return size;
	}
};
class PacketBase
{
public:
	uint8_t type;
	uint8_t senderId;

	virtual size_t GetSize()
	{
		return sizeof(uint8_t) + sizeof(uint8_t);
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	virtual void* Serialize()
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];

		size_t offset = 0;
		SERIALIZE_BASE_PACKET();
		return buffer;
	}
	static PacketBase DeserializePacket(const void* data, size_t size)
	{
		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		if (size < sizeof(type))
			throw std::runtime_error("Packet too small");
		PacketBase packet;
		PacketType type = PacketType();
		size_t offset = 0;

		DESERIALIZE_BASE_PACKET();
		return packet;
	}
};
class GenericPacketString : public PacketBase
{
public:
	std::string payload;
	size_t GetSize() override
	{
		return 2 + payload.size() + 1;
	}
	void SetString(std::string data)
	{
		payload = data;
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];

		size_t offset = 0;
		SERIALIZE_BASE_PACKET();

		PacketCommon::SerializeStdString(buffer, offset, payload);
		//memcpy(buffer + offset, payload.c_str(), payload.size() + 1); // include null terminator

		return buffer; // caller must delete[] this
	}
	static GenericPacketString Deserialize(void* data, size_t size)
	{
		uint8_t* buffer = static_cast<uint8_t*>(data);
		size_t offset = 0;

		GenericPacketString packet;
		DESERIALIZE_BASE_PACKET();

		PacketCommon::DeserializeStdString(buffer, offset, packet.payload, size);

		return packet;
	}
};
class GenericPacketVec4 : public PacketBase
{
public:
	float x;
	float y;
	float z;
	float w;
	size_t GetSize() override
	{
		return 2 + sizeof(float) * 5;
	}
#if BBM_CLIENT
	void Set(hh::math::CVector& in_Vec3)
	{
		x = in_Vec3.x();
		y = in_Vec3.y();
		z = in_Vec3.z();
	}
	void Set(hh::math::CVector4& in_Vec4)
	{
		x = in_Vec4.x();
		y = in_Vec4.y();
		z = in_Vec4.z();
		w = in_Vec4.w();
	}
	void Set(hh::math::CVector2& in_Vec4)
	{
		x = in_Vec4.x();
		y = in_Vec4.y();
	}
#endif
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];

		size_t offset = 0;
		SERIALIZE_BASE_PACKET();

		PacketCommon::SerializeGeneric(buffer, offset, x);
		PacketCommon::SerializeGeneric(buffer, offset, y);
		PacketCommon::SerializeGeneric(buffer, offset, z);
		PacketCommon::SerializeGeneric(buffer, offset, w);

		return buffer; // caller must delete[] this
	}
	static GenericPacketVec4 Deserialize(const void* data, size_t size)
	{
		if (size < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(float) * 4)
			throw std::runtime_error("GenericPacketVec4: Packet too small");

		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = GenericPacketVec4();

		DESERIALIZE_BASE_PACKET();

		PacketCommon::DeserializeGeneric(buffer, offset, packet.x);
		PacketCommon::DeserializeGeneric(buffer, offset, packet.y);
		PacketCommon::DeserializeGeneric(buffer, offset, packet.z);
		PacketCommon::DeserializeGeneric(buffer, offset, packet.w);
		return packet;
	}
};
class GenericPacketVec4Quantized : public PacketBase
{
public:
	int8_t x;
	int8_t y;
	int8_t z;
	int8_t w;
	size_t GetSize() override
	{
		return 2 + sizeof(uint8_t) * 5;
	}
	int8_t QuantizeToByte(float value, float min, float max)
	{
		float clamped = std::clamp(value, min, max);
		float normalized = (clamped - min) / (max - min);
		return (int8_t)(normalized * 255.0f);
	}
	float DequantizeFromByte(int8_t data, float min, float max)
	{
		float normalized = data / 255.0f;
		return normalized * (max - min) + min;
	}
#if BBM_CLIENT
	void Set(hh::math::CVector& in_Vec3, float min, float max)
	{
		x = QuantizeToByte(in_Vec3.x(), min, max);
		y = QuantizeToByte(in_Vec3.y(), min, max);
		z = QuantizeToByte(in_Vec3.z(), min, max);
	}
#endif
	SimpleVec4 GetAsFloat(float min, float max)
	{
		return SimpleVec4(DequantizeFromByte(x, min, max), DequantizeFromByte(y, min, max), DequantizeFromByte(z, min, max));
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];

		size_t offset = 0;
		SERIALIZE_BASE_PACKET();

		PacketCommon::SerializeGeneric(buffer, offset, x);
		PacketCommon::SerializeGeneric(buffer, offset, y);
		PacketCommon::SerializeGeneric(buffer, offset, z);
		PacketCommon::SerializeGeneric(buffer, offset, w);

		return buffer; // caller must delete[] this
	}
	static GenericPacketVec4Quantized Deserialize(const void* data, size_t size)
	{
		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = GenericPacketVec4Quantized();

		DESERIALIZE_BASE_PACKET();

		PacketCommon::DeserializeGeneric(buffer, offset, packet.x);
		PacketCommon::DeserializeGeneric(buffer, offset, packet.y);
		PacketCommon::DeserializeGeneric(buffer, offset, packet.z);
		PacketCommon::DeserializeGeneric(buffer, offset, packet.w);
		return packet;
	}
};
class GenericPacketInt64 : public PacketBase
{
public:
	int64_t payload;
	size_t GetSize() override
	{
		return 2 + sizeof(int64_t);
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];

		size_t offset = 0;
		SERIALIZE_BASE_PACKET();
		PacketCommon::SerializeGeneric(buffer, offset, payload);
		return buffer;
	}

	static GenericPacketInt64 Deserialize(const void* data, size_t size)
	{
		if (size < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(int))
			throw std::runtime_error("GenericPacketInt: Packet too small");

		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = GenericPacketInt64();
		DESERIALIZE_BASE_PACKET();
		PacketCommon::DeserializeGeneric(buffer, offset, packet.payload);
		return packet;
	}
};
class GenericPacketInt : public PacketBase
{
public:
	int payload;
	size_t GetSize() override
	{
		return 2 + sizeof(int);
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];

		size_t offset = 0;
		SERIALIZE_BASE_PACKET();
		PacketCommon::SerializeGeneric(buffer, offset, payload);
		return buffer;
	}

	static GenericPacketInt Deserialize(const void* data, size_t size)
	{
		if (size < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(int))
			throw std::runtime_error("GenericPacketInt: Packet too small");

		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = GenericPacketInt();
		DESERIALIZE_BASE_PACKET();
		PacketCommon::DeserializeGeneric(buffer, offset, packet.payload);
		return packet;
	}
};
class GenericPacketFloat : public PacketBase
{
public:
	float payload;
	size_t GetSize() override
	{
		return 2 + sizeof(float);
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];

		size_t offset = 0;
		SERIALIZE_BASE_PACKET();
		PacketCommon::SerializeGeneric(buffer, offset, payload);
		return buffer;
	}

	static GenericPacketFloat Deserialize(const void* data, size_t size)
	{
		if (size < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(int))
			throw std::runtime_error("GenericPacketFloat: Packet too small");

		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = GenericPacketFloat();
		DESERIALIZE_BASE_PACKET();
		PacketCommon::DeserializeGeneric(buffer, offset, packet.payload);
		return packet;
	}
};
class DamagePacket : public PacketBase
{
public:
	int targetClient;
	std::string senderState;

	size_t GetSize() override
	{
		return sizeof(uint8_t) + sizeof(uint8_t) + sizeof(int) + sizeof(uint32_t) + senderState.size(); // type + senderId + int
	}

	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];
		size_t offset = 0;

		SERIALIZE_BASE_PACKET();
		PacketCommon::SerializeGeneric(buffer, offset, targetClient);
		PacketCommon::SerializeStdString(buffer, offset, senderState);

		return buffer;
	}

	static DamagePacket Deserialize(const void* data, size_t size)
	{
		if (size < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(int))
			throw std::runtime_error("GenericPacketInt: Packet too small");

		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = DamagePacket();
		DESERIALIZE_BASE_PACKET();
		PacketCommon::DeserializeGeneric(buffer, offset, packet.targetClient);
		PacketCommon::DeserializeStdString(buffer, offset, packet.senderState, size);

		return packet;
	}
};
class AnimPacket : public PacketBase
{
public:
	float speed;
	std::string animName;

	AnimPacket()
	{
		type = PacketType::ANIMATE;
	}
	size_t GetSize() override
	{
		return sizeof(uint8_t) + sizeof(uint8_t) + sizeof(float) + sizeof(uint32_t) + animName.size(); // type + senderId + int
	}

	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];
		size_t offset = 0;

		SERIALIZE_BASE_PACKET();
		PacketCommon::SerializeGeneric(buffer, offset, speed);
		PacketCommon::SerializeStdString(buffer, offset, animName);

		return buffer;
	}

	static AnimPacket Deserialize(const void* data, size_t size)
	{
		if (size < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(float))
			throw std::runtime_error("GenericPacketInt: Packet too small");

		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = AnimPacket();
		DESERIALIZE_BASE_PACKET();
		PacketCommon::DeserializeGeneric(buffer, offset, packet.speed);
		PacketCommon::DeserializeStdString(buffer, offset, packet.animName, size);

		return packet;
	}
};
class ChatPacket : public PacketBase
{
public:
	uint8_t chatType;
	std::string chatMessage;

	size_t GetSize() override
	{
		return sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint32_t) + chatMessage.size(); // type + senderId + int
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];
		size_t offset = 0;

		SERIALIZE_BASE_PACKET();
		PacketCommon::SerializeGeneric(buffer, offset, chatType);
		PacketCommon::SerializeStdString(buffer, offset, chatMessage);

		return buffer;
	}

	static ChatPacket Deserialize(const void* data, size_t size)
	{
		if (size < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(int))
			throw std::runtime_error("GenericPacketInt: Packet too small");

		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = ChatPacket();
		DESERIALIZE_BASE_PACKET();
		PacketCommon::DeserializeGeneric(buffer, offset, packet.chatType);
		PacketCommon::DeserializeStdString(buffer, offset, packet.chatMessage, size);

		return packet;
	}
};
class SyncClientsPacket : public PacketBase
{
public:
	std::vector<SClientCommonData> clientIds;
	uint16_t maximumPeersCount;
	//std::vector<std::string> clientNames;
	//std::vector<std::string> clientStages;
	//std::vector<SimpleVec4> clientPositions;
	//std::vector<SimpleVec4> clientRotations;
	//std::vector<int> clientPlayerTypes;

	SyncClientsPacket()
	{
		type = SYNC_USERS;
	}
	size_t GetSize() override
	{
		size_t size = sizeof(uint8_t) + sizeof(uint8_t); // type + senderId
		


		size += PacketCommon::GetClientCommonDataVectorSize(clientIds);
		size += sizeof(uint16_t);
		//size += PacketCommon::GetStringVectorSize(clientNames);
		//size += PacketCommon::GetStringVectorSize(clientStages);
		//size += PacketCommon::GetVectorSize(clientPositions);
		//size += PacketCommon::GetVectorSize(clientRotations);
		//size += PacketCommon::GetVectorSize(clientPlayerTypes);
		return size;
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];
		size_t offset = 0;

		SERIALIZE_BASE_PACKET();

		PacketCommon::SerializeClientVector(clientIds, buffer, offset);
		PacketCommon::SerializeGeneric(buffer, offset, maximumPeersCount);
		//PacketCommon::SerializeStringVector(clientNames, buffer, offset);
		//PacketCommon::SerializeStringVector(clientStages, buffer, offset);
		//PacketCommon::SerializeVector(clientPositions, buffer, offset);
		//PacketCommon::SerializeVector(clientRotations, buffer, offset);
		//PacketCommon::SerializeVector(clientPlayerTypes, buffer, offset);

		return buffer;
	}

	static SyncClientsPacket Deserialize(const void* data, size_t size)
	{
		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		if (size < sizeof(PacketType) + sizeof(uint8_t))
			throw std::runtime_error("Packet too small");

		SyncClientsPacket packet;
		DESERIALIZE_BASE_PACKET();

		PacketCommon::DeserializeClientVector(packet.clientIds, buffer, offset);
		PacketCommon::DeserializeGeneric(buffer, offset, packet.maximumPeersCount);
		//PacketCommon::DeserializeStringVector(packet.clientNames, buffer, offset, size);
		//PacketCommon::DeserializeStringVector(packet.clientStages, buffer, offset, size);
		//PacketCommon::DeserializeVector(packet.clientPositions, buffer, offset);
		//PacketCommon::DeserializeVector(packet.clientRotations, buffer, offset);
		//PacketCommon::DeserializeVector(packet.clientPlayerTypes, buffer, offset);

		return packet;
	}
};
class StageEndPacket : public PacketBase
{
public:
	float timeMilliseconds;
	std::string stage;

	StageEndPacket()
	{
		type = PacketType::STAGE_END;
	}
	size_t GetSize() override
	{
		return sizeof(uint8_t) + sizeof(uint8_t) +
			sizeof(uint32_t) + stage.size() +   // string length prefix + data
			(sizeof(float) * 2);                      // float
	}
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];
		size_t offset = 0;

		SERIALIZE_BASE_PACKET();

		// Serialize float
		PacketCommon::SerializeGeneric<float>(buffer, offset, timeMilliseconds);

		// Serialize string
		PacketCommon::SerializeStdString(buffer, offset, stage);

		return buffer;
	}
	static StageEndPacket Deserialize(const void* data, size_t size)
	{
		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		if (size < sizeof(PacketType) + sizeof(uint8_t))
			throw std::runtime_error("Packet too small");

		StageEndPacket packet;
		DESERIALIZE_BASE_PACKET();

		PacketCommon::DeserializeGeneric<float>(buffer, offset, packet.timeMilliseconds);
		PacketCommon::DeserializeStdString(buffer, offset, packet.stage, size);

		return packet;
	}
};
class AuthenticationPacket : public PacketBase
{
public:
	int version;
	uint8_t requestResult = 0;
	std::string modList;

	AuthenticationPacket()
	{
		type = PacketType::SYNC_CLIENT_INFO;
	}
	size_t GetSize() override
	{
		return sizeof(uint8_t) + sizeof(uint8_t) +
			sizeof(uint32_t) + modList.size() +   // string length prefix + data
			(sizeof(int) * 2) + sizeof(uint8_t);                      // float
	}
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];
		size_t offset = 0;

		SERIALIZE_BASE_PACKET();

		// Serialize float
		PacketCommon::SerializeGeneric<int>(buffer, offset, version);

		PacketCommon::SerializeGeneric<uint8_t>(buffer, offset, requestResult);
		// Serialize string
		PacketCommon::SerializeStdString(buffer, offset, modList);

		return buffer;
	}
	static AuthenticationPacket Deserialize(const void* data, size_t size)
	{
		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		if (size < sizeof(PacketType) + sizeof(uint8_t))
			throw std::runtime_error("Packet too small");

		AuthenticationPacket packet;
		DESERIALIZE_BASE_PACKET();

		PacketCommon::DeserializeGeneric<int>(buffer, offset, packet.version);
		PacketCommon::DeserializeGeneric<uint8_t>(buffer, offset, packet.requestResult);

		PacketCommon::DeserializeStdString(buffer, offset, packet.modList, size);

		return packet;
	}
};
class ServerSettingsPacket : public PacketBase
{
public:
	bool DoPlayerDamageHoming;
	bool DoPlayerDamageStomp;
	bool DoPlayerPushBoost;

	ServerSettingsPacket()
	{
		type = PacketType::SYNC_CLIENT_INFO;
	}
	size_t GetSize() override
	{
		return PacketBase::GetSize()
			+ sizeof(bool)
			+ sizeof(bool)
			+ sizeof(bool);
	}
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];
		size_t offset = 0;

		SERIALIZE_BASE_PACKET();

		// Serialize float
		PacketCommon::SerializeGeneric<bool>(buffer, offset, DoPlayerDamageHoming);

		PacketCommon::SerializeGeneric<bool>(buffer, offset, DoPlayerDamageStomp);
		PacketCommon::SerializeGeneric<bool>(buffer, offset, DoPlayerPushBoost);
		// Serialize string

		return buffer;
	}
	static ServerSettingsPacket Deserialize(const void* data, size_t size)
	{
		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		if (size < sizeof(PacketType) + sizeof(uint8_t))
			throw std::runtime_error("Packet too small");

		ServerSettingsPacket packet;
		DESERIALIZE_BASE_PACKET();

		PacketCommon::DeserializeGeneric<bool>(buffer, offset, packet.DoPlayerDamageHoming);
		PacketCommon::DeserializeGeneric<bool>(buffer, offset, packet.DoPlayerDamageStomp);
		PacketCommon::DeserializeGeneric<bool>(buffer, offset, packet.DoPlayerPushBoost);
		return packet;
	}
};
class StageStartPacket : public PacketBase
{
public:
	uint8_t playerType;
	std::string stage;

	size_t GetSize() override
	{
		return sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint32_t) + stage.size(); // type + senderId + int
	}
	/// Call "delete[] static_cast<uint8_t*>(packetData);" after this everytime.
	void* Serialize() override
	{
		size_t size = GetSize();
		uint8_t* buffer = new uint8_t[size];
		size_t offset = 0;

		SERIALIZE_BASE_PACKET();
		PacketCommon::SerializeGeneric(buffer, offset, playerType);
		PacketCommon::SerializeStdString(buffer, offset, stage);

		return buffer;
	}
	static StageStartPacket Deserialize(const void* data, size_t size)
	{
		if (size < sizeof(uint8_t) + sizeof(uint8_t) + sizeof(int))
			throw std::runtime_error("GenericPacketInt: Packet too small");

		const uint8_t* buffer = static_cast<const uint8_t*>(data);
		size_t offset = 0;

		auto packet = StageStartPacket();
		DESERIALIZE_BASE_PACKET();
		PacketCommon::DeserializeGeneric(buffer, offset, packet.playerType);
		PacketCommon::DeserializeStdString(buffer, offset, packet.stage, size);

		return packet;
	}
};