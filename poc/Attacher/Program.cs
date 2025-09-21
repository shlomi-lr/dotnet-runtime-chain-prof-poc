using System;
using Microsoft.Diagnostics.NETCore.Client;
using System.IO;
using System.Threading.Tasks;
class P {
  static int Main(string[] args){
    if (args.Length<3){ Console.WriteLine("usage: Attacher <pid> <guid> <path>"); return 2; }
    int pid=int.Parse(args[0]); var guid=args[1]; var path=args[2];
    var client = new DiagnosticsClient(pid);
    var g = new Guid(guid);
    byte[] clientData = Array.Empty<byte>();
    try{
      client.AttachProfiler(TimeSpan.FromSeconds(10), g, path, clientData);
      Console.WriteLine("attach: OK");
      return 0;
    }catch(Exception ex){ Console.WriteLine("attach: FAIL: "+ex.GetType().Name+" "+ex.Message); return 1; }
  }
}
