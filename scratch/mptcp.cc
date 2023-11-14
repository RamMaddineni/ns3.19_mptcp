/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Morteza Kheirkhah <m.kheirkhah@sussex.ac.uk>
 */

// Network topology
//
//       n0 ----------- n1
// - Flow from n0 to n1 using MpTcpBulkSendApplication.

#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("MpTcpBulkSendExample");


// double old_time = 0.0;
// EventId output;
// Time current = Time::FromInteger(3, Time::S);  //Only record cwnd and ssthresh values every 3 seconds
// bool first = true;

// static void
// OutputTrace ()
// {
//  // *stream->GetStream() << newtime << " " << newval << std::endl;
//  // old_time = newval;
// }
// static void
// CwndTracer (Ptr<OutputStreamWrapper>stream, uint32_t oldval, uint32_t newval)
// {
//   double new_time = Simulator::Now().GetSeconds();
//   if (old_time == 0 && first)
//   {
//     double mycurrent = current.GetSeconds();
//     *stream->GetStream() << new_time << " " << mycurrent << " " << newval << std::endl;
//     first = false;
//     output = Simulator::Schedule(current,&OutputTrace);
//   }
//   else
//   {
//     if (output.IsExpired())
//     {
//       *stream->GetStream() << new_time << " " << newval << std::endl;
//       output.Cancel();
//       output = Simulator::Schedule(current,&OutputTrace);
//     }
//   }
// }

// static void
// SsThreshTracer (Ptr<OutputStreamWrapper>stream, uint32_t oldval, uint32_t newval)
// {
//   double new_time = Simulator::Now().GetSeconds();
//   if (old_time == 0 && first)
//   {
//     double mycurrent = current.GetSeconds();
//     *stream->GetStream() << new_time << " " << mycurrent << " " << newval << std::endl;
//     first = false;
//     output = Simulator::Schedule(current,&OutputTrace);
//   }
//   else
//   {
//     if (output.IsExpired())
//     {
//       *stream->GetStream() << new_time << " " << newval << std::endl;
//       output.Cancel();
//       output = Simulator::Schedule(current,&OutputTrace);
//     }
//   }
// }

// static void
// TraceCwnd (std::string cwnd_tr_file_name)
// {
//   AsciiTraceHelper ascii;
//   if (cwnd_tr_file_name.compare("") == 0)
//      {
//        NS_LOG_DEBUG ("No trace file for cwnd provided");
//        return;
//      }
//   else
//     {
//       Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream(cwnd_tr_file_name.c_str());
//       Config::ConnectWithoutContext ("/NodeList/1/$ns3::MpTcpSocketBase/SocketList/0/",MakeBoundCallback (&CwndTracer, stream));
//     }
// }

// static void
// TraceSsThresh(std::string ssthresh_tr_file_name)
// {
//   AsciiTraceHelper ascii;
//   if (ssthresh_tr_file_name.compare("") == 0)
//     {
//       NS_LOG_DEBUG ("No trace file for ssthresh provided");
//       return;
//     }
//   else
//     {
//       Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream(ssthresh_tr_file_name.c_str());
//       Config::ConnectWithoutContext ("/NodeList/1/$ns3::MpTcpSocketBase/SocketList/0/SlowStartThreshold",MakeBoundCallback (&SsThreshTracer, stream));
//     }
// }

int
main(int argc, char *argv[])
{
  LogComponentEnable("MpTcpSocketBase", LOG_INFO);

  Config::SetDefault("ns3::Ipv4GlobalRouting::FlowEcmpRouting", BooleanValue(true));
  Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(1400));
  Config::SetDefault("ns3::TcpSocket::DelAckCount", UintegerValue(0));
  Config::SetDefault("ns3::DropTailQueue::Mode", StringValue("QUEUE_MODE_PACKETS"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", UintegerValue(100));
  Config::SetDefault("ns3::TcpL4Protocol::SocketType", TypeIdValue(MpTcpSocketBase::GetTypeId()));
  Config::SetDefault("ns3::MpTcpSocketBase::MaxSubflows", UintegerValue(8)); // Sink
  Config::SetDefault("ns3::MpTcpSocketBase::CongestionControl", StringValue("RTT_Compensator"));
  Config::SetDefault("ns3::MpTcpSocketBase::PathManagement", StringValue("NdiffPorts"));
  // Config::SetDefault("ns3::MpTcpSocketBase::shortPlotting", BooleanValue(true));  
  NodeContainer nodes;
  nodes.Create(2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
  pointToPoint.SetChannelAttribute("Delay", StringValue("1ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install(nodes);

  InternetStackHelper internet;
  internet.Install(nodes);

  Ipv4AddressHelper ipv4;
  ipv4.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign(devices);

  uint16_t port = 9;
  MpTcpPacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
  ApplicationContainer sinkApps = sink.Install(nodes.Get(1));
  sinkApps.Start(Seconds(0.0));
  sinkApps.Stop(Seconds(15.0));

  MpTcpBulkSendHelper source("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address(i.GetAddress(1)), port));
  source.SetAttribute("MaxBytes", UintegerValue(0));
  ApplicationContainer sourceApps = source.Install(nodes.Get(0));
  sourceApps.Start(Seconds(0.0));
  sourceApps.Stop(Seconds(15.0));

  // bool tracing =true;
  // std::string tr_file_name="mptcp_trace_file";
  // std::string ssthresh_tr_file_name="mptcp_ssthresh";
  // std::string cwnd_tr_file_name="mptcp_cwnd";
  // // InternetStackHelper stack;
  // // stack.InstallAll ();
  // if (tracing)
  // {
  
  //   std::ofstream ascii;
  //   printf("came here");
  //   Ptr<OutputStreamWrapper> ascii_wrap;
  //   printf("Line 323");
  //   if (tr_file_name.compare("") == 0)
  //     {
  //       NS_LOG_DEBUG ("No trace file provided");
  //       exit (1);
  //     }
  //   else
  //     {
  //       ascii.open (tr_file_name.c_str());
  //       ascii_wrap = new OutputStreamWrapper(tr_file_name.c_str(), std::ios::out);
  //     }
  //   printf("Line 334");
  //   // stack.EnableAsciiIpv4All (ascii_wrap);

  //   Simulator::Schedule(Seconds(0.00001), &TraceCwnd, cwnd_tr_file_name);
  //   Simulator::Schedule(Seconds(0.00001), &TraceSsThresh, ssthresh_tr_file_name);
  //     printf("came here 2");
  // }
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop(Seconds(20.0));
  Simulator::Run();
  Simulator::Destroy();
  NS_LOG_INFO ("Done.");

}
