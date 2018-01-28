#ifndef MS_RTC_PLAIN_RTPWEBRTC_TRANSPORT_HPP
#define MS_RTC_PLAIN_RTPWEBRTC_TRANSPORT_HPP

#include "common.hpp"
#include "Channel/Notifier.hpp"
#include "RTC/TransportLoopback.hpp"
#include <json/json.h>
#include <string>

namespace RTC
{
	class PlainRtpTransport : public RTC::TransportLoopback
	{
	public:
		struct Options
		{
			std::string remoteIP;
			uint16_t remotePort;
		};

	public:
		PlainRtpTransport(
		  RTC::TransportLoopback::Listener* listener,
		  Channel::Notifier* notifier,
		  uint32_t transportId,
		  Options& options);

	private:
		~PlainRtpTransport();

	public:
		Json::Value ToJson() const override;
		Json::Value GetStats() const override;
		void SendRtpPacket(RTC::RtpPacket* packet) override;
		void SendRtcpPacket(RTC::RTCP::Packet* packet) override;

	private:
		bool IsConnected() const override;
		void SendRtcpCompoundPacket(RTC::RTCP::CompoundPacket* packet) override;

		/* Private methods to unify UDP and TCP behavior. */
	private:
		void OnPacketRecv(RTC::TransportTuple* tuple, const uint8_t* data, size_t len);
		void OnRtpDataRecv(RTC::TransportTuple* tuple, const uint8_t* data, size_t len);
		void OnRtcpDataRecv(RTC::TransportTuple* tuple, const uint8_t* data, size_t len);

		/* Pure virtual methods inherited from RTC::UdpSocketLoopback::Listener. */
	public:
		void OnPacketRecv(
		  RTC::UdpSocketLoopback* socket,
		  const uint8_t* data,
		  size_t len,
		  const struct sockaddr* remoteAddr) override;

	private:
		// Allocated by this.
		RTC::UdpSocketLoopback* udpSocket{ nullptr };
		RTC::TransportTuple* tuple{ nullptr };
		// Others.
		struct sockaddr_storage remoteAddrStorage
		{
		};
	};
} // namespace RTC

#endif
