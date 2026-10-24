// Prints "<windowID> <width>x<height> <title>" for each on-screen normal window of the given pid.
import CoreGraphics
import Foundation
let pid = Int32(CommandLine.arguments[1])!
let list = CGWindowListCopyWindowInfo([.optionOnScreenOnly, .excludeDesktopElements], kCGNullWindowID) as! [[String: Any]]
for w in list where (w[kCGWindowOwnerPID as String] as? Int32) == pid && (w[kCGWindowLayer as String] as? Int) == 0 {
  let b = w[kCGWindowBounds as String] as? [String: CGFloat] ?? [:]
  print("\(w[kCGWindowNumber as String]!) \(Int(b["Width"] ?? 0))x\(Int(b["Height"] ?? 0)) \(w[kCGWindowName as String] as? String ?? "")")
}
