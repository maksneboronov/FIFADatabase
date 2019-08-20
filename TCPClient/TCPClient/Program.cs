using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace TCPClient
{
	class Program
    {
		public static async Task<IPHostEntry> GetHost(string str)
		{
			return await Dns.GetHostEntryAsync(str);
		}

		static void SendMessageFromSocket(int port)
		{
			byte[] bytes = new byte[1024];

			var ipHost = GetHost("localhost").Result;
			var ipAddr = ipHost.AddressList[0];
			var ipEndPoint = new IPEndPoint(ipAddr, port);

			using (var sender = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp))
			{
				sender.Connect(ipEndPoint);
				sender.ReceiveTimeout = 1000 * 5;

				while (true)
				{
					Console.Write("Enter: ");
					var message = Console.ReadLine().ToLower();
					var bytesSent = sender.Send(Encoding.UTF8.GetBytes(message));
					try
					{
						switch (message)
						{
							case "backup":								
								var bytesRec = sender.Receive(bytes);
								var size = BitConverter.ToInt64(bytes, 0);

								using (var file = new FileStream("1.bak", FileMode.Create, FileAccess.Write))
								{
									using (var f = new BinaryWriter(file))
									{
										var i = 0;
										while (i < size)
										{
											var br = sender.Receive(bytes);
											f.Write(bytes, 0, br);
											i += br;
										}
									}
								}
								break;
							case "restore":
								using (var file = new FileStream("1.bak", FileMode.Open, FileAccess.Read, FileShare.Read))
								{
									sender.Send(BitConverter.GetBytes(file.Length));
									using (var f = new BinaryReader(file))
									{
										while (file.Position < file.Length)
										{
											var res = f.ReadBytes(1024);
											sender.Send(res, res.Length, SocketFlags.None);
										}
									}
								}
								break;
						}
						
					}
					catch
					{
						break;
					}

					if (message.IndexOf("<TheEnd>") != -1)
					{
						break;
					}
				}
				sender.Shutdown(SocketShutdown.Both);
			}
		}

		static void Main(string[] args)
        {
			try
			{
				SendMessageFromSocket(11000);
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.ToString());
			}
			finally
			{
				Console.ReadLine();
			}
		}
    }
}
