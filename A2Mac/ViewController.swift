//
//  ViewController.swift
//  A2Mac
//
//  Created by Tamas Rudnai on 7/25/19.
//  Copyright © 2019 GameAlloy. All rights reserved.
//

import Cocoa

class ViewController: NSViewController {

    @IBOutlet weak var displayField: NSTextField!
    @IBOutlet weak var display: NSTextFieldCell!
    @IBOutlet weak var speedometer: NSTextFieldCell!
    
//    static let charConvStr : String =
//        "@🄰🄱🄲🄳🄴🄵🄶🄷🄸🄹🄺🄻🄼🄽🄾🄿🅀🅁🅂🅃🅄🅅🅆🅇🅈🅉[\\]^_ !\"#$%&'()*+,-./0123456789:;<=>?" +
//        "@🅰🅱🅲🅳🅴🅵🅶🅷🅸🅹🅺🅻🅼🅽🅾🅿🆀🆁🆂🆃🆄🆅🆆🆇🆈🆉[\\]^_⬛︎!\"#$%&'()*+,-./0123456789:;<=>?" + // FL
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ !\"#$%&'()*+,-./0123456789:;<=>?" +
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~?"

//    static let charConvStr : String =
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ !\"#$%&'()*+,-./0123456789:;<=>?" +
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_░!\"#$%&'()*+,-./0123456789:;<=>?" + // FL
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\u{E0A0}!\"#$%&'()*+,-./0123456789:;<=>?" +
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~?"
    
//    static let charConvStr : String =
//    "\u{E140}\u{E141}\u{E142}\u{E143}\u{E144}\u{E145}\u{E146}\u{E147}\u{E148}\u{E149}\u{E14A}\u{E14B}\u{E14C}\u{E14D}\u{E14E}\u{E14F}\u{E150}\u{E151}\u{E152}\u{E153}\u{E154}\u{E155}\u{E156}\u{E157}\u{E158}\u{E159}\u{E15A}\u{E15B}\u{E15C}\u{E15D}\u{E15E}\u{E15F}\u{E120}\u{E121}\u{E122}\u{E123}\u{E124}\u{E125}\u{E126}\u{E127}\u{E128}\u{E129}\u{E12A}\u{E12B}\u{E12C}\u{E12D}\u{E12E}\u{E12F}\u{E130}\u{E131}\u{E132}\u{E133}\u{E134}\u{E135}\u{E136}\u{E137}\u{E138}\u{E139}\u{E13A}\u{E13B}\u{E13C}\u{E13D}\u{E13E}\u{E13F}" +
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_░!\"#$%&'()*+,-./0123456789:;<=>?" + // FL
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\u{E0A0}!\"#$%&'()*+,-./0123456789:;<=>?" +
//        "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~?"
    
    static let charConvStrFlashOff : String =
        "\u{E140}\u{E141}\u{E142}\u{E143}\u{E144}\u{E145}\u{E146}\u{E147}\u{E148}\u{E149}\u{E14A}\u{E14B}\u{E14C}\u{E14D}\u{E14E}\u{E14F}\u{E150}\u{E151}\u{E152}\u{E153}\u{E154}\u{E155}\u{E156}\u{E157}\u{E158}\u{E159}\u{E15A}\u{E15B}\u{E15C}\u{E15D}\u{E15E}\u{E15F}\u{E120}\u{E121}\u{E122}\u{E123}\u{E124}\u{E125}\u{E126}\u{E127}\u{E128}\u{E129}\u{E12A}\u{E12B}\u{E12C}\u{E12D}\u{E12E}\u{E12F}\u{E130}\u{E131}\u{E132}\u{E133}\u{E134}\u{E135}\u{E136}\u{E137}\u{E138}\u{E139}\u{E13A}\u{E13B}\u{E13C}\u{E13D}\u{E13E}\u{E13F}" +
            "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ !\"#$%&'()*+,-./0123456789:;<=>?" + // FL
            "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\u{E0A0}!\"#$%&'()*+,-./0123456789:;<=>?" +
    "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~?"

    static let charConvStrFlashOn : String =
        "\u{E140}\u{E141}\u{E142}\u{E143}\u{E144}\u{E145}\u{E146}\u{E147}\u{E148}\u{E149}\u{E14A}\u{E14B}\u{E14C}\u{E14D}\u{E14E}\u{E14F}\u{E150}\u{E151}\u{E152}\u{E153}\u{E154}\u{E155}\u{E156}\u{E157}\u{E158}\u{E159}\u{E15A}\u{E15B}\u{E15C}\u{E15D}\u{E15E}\u{E15F}\u{E120}\u{E121}\u{E122}\u{E123}\u{E124}\u{E125}\u{E126}\u{E127}\u{E128}\u{E129}\u{E12A}\u{E12B}\u{E12C}\u{E12D}\u{E12E}\u{E12F}\u{E130}\u{E131}\u{E132}\u{E133}\u{E134}\u{E135}\u{E136}\u{E137}\u{E138}\u{E139}\u{E13A}\u{E13B}\u{E13C}\u{E13D}\u{E13E}\u{E13F}" +
        "\u{E140}\u{E141}\u{E142}\u{E143}\u{E144}\u{E145}\u{E146}\u{E147}\u{E148}\u{E149}\u{E14A}\u{E14B}\u{E14C}\u{E14D}\u{E14E}\u{E14F}\u{E150}\u{E151}\u{E152}\u{E153}\u{E154}\u{E155}\u{E156}\u{E157}\u{E158}\u{E159}\u{E15A}\u{E15B}\u{E15C}\u{E15D}\u{E15E}\u{E15F}\u{E120}\u{E121}\u{E122}\u{E123}\u{E124}\u{E125}\u{E126}\u{E127}\u{E128}\u{E129}\u{E12A}\u{E12B}\u{E12C}\u{E12D}\u{E12E}\u{E12F}\u{E130}\u{E131}\u{E132}\u{E133}\u{E134}\u{E135}\u{E136}\u{E137}\u{E138}\u{E139}\u{E13A}\u{E13B}\u{E13C}\u{E13D}\u{E13E}\u{E13F}" +
            "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\u{E0A0}!\"#$%&'()*+,-./0123456789:;<=>?" +
    "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~?"

    static var charConvTbl = Array( charConvStrFlashOff )

    let textLineOfs : [Int] = [
        0x000, 0x080, 0x100, 0x180, 0x200, 0x280, 0x300, 0x380, 0x028, 0x0A8, 0x128, 0x1A8,
        0x228, 0x2A8, 0x328, 0x3A8, 0x050, 0x0D0, 0x150, 0x1D0, 0x250, 0x2D0, 0x350, 0x3D0
    ]
    
    var workItem : DispatchWorkItem? = nil;
    @IBAction func Power(_ sender: Any) {
        #if SPEEDTEST
        if ( workItem != nil ) {
            workItem!.cancel();
            workItem = nil;
        }
        else {
            workItem = DispatchWorkItem {
                DispatchQueue.global(qos: .userInteractive).async {
//                DispatchQueue.global(qos: .userInitiated).async {
//                DispatchQueue.global(qos: .background).async {
                    tst6502()
                }
            }
            DispatchQueue.global().async(execute: workItem!);
        }
        #else
        m6502_Reset()
        #endif
    }
    
    @IBAction func Reset(_ sender: Any) {
//        let resetPointer = UnsafeRawBufferPointer(start: &RAM + 0x3F2, count: 2)
//        let ral = UInt16(resetPointer[0])
//        let rah = UInt16(resetPointer[1])
//        let resetAddr = rah << 8 + ral
//
//        let hex = String(resetAddr, radix: 16, uppercase: true)
//        print("reset to:\(hex)\n")
//        m6502.pc = resetAddr
        
        m6502.interrupt = SOFTRESET;
    }
    
    
    override func keyDown(with event: NSEvent) {
//        switch event.modifierFlags.intersection(.deviceIndependentFlagsMask) {
//        case [.command] where event.characters == "l",
//             [.command, .shift] where event.characters == "l":
//            print("command-l or command-shift-l")
//        default:
//            break
//        }
//        print( "key = " + (event.charactersIgnoringModifiers ?? ""))
//        print( "\ncharacter = " + (event.characters ?? ""))
        
        #if FUNCTIONTEST
        #else
        if let chars = event.characters {
            let char = chars.uppercased()[chars.startIndex]
            if let code = char.asciiValue {
                var A2code = code | 0x80
                
                switch ( code ) {
                case 13:
                    A2code = 141
                    break
                    
                default:
                    break
                }
//                print("keycode: \(code) --> \(A2code)")
                
                let kbdPointer = UnsafeMutableRawBufferPointer(start: &RAM + 0xC000, count: 1)
                kbdPointer[0] = A2code
            }
        }
        #endif
        
    }
    
    override func flagsChanged(with event: NSEvent) {
//        switch event.modifierFlags.intersection(.deviceIndependentFlagsMask) {
//        case [.shift]:
//            print("shift key is pressed")
//        case [.control]:
//            print("control key is pressed")
//        case [.option] :
//            print("option key is pressed")
//        case [.command]:
//            print("Command key is pressed")
//        case [.control, .shift]:
//            print("control-shift keys are pressed")
//        case [.option, .shift]:
//            print("option-shift keys are pressed")
//        case [.command, .shift]:
//            print("command-shift keys are pressed")
//        case [.control, .option]:
//            print("control-option keys are pressed")
//        case [.control, .command]:
//            print("control-command keys are pressed")
//        case [.option, .command]:
//            print("option-command keys are pressed")
//        case [.shift, .control, .option]:
//            print("shift-control-option keys are pressed")
//        case [.shift, .control, .command]:
//            print("shift-control-command keys are pressed")
//        case [.control, .option, .command]:
//            print("control-option-command keys are pressed")
//        case [.shift, .command, .option]:
//            print("shift-command-option keys are pressed")
//        case [.shift, .control, .option, .command]:
//            print("shift-control-option-command keys are pressed")
//        default:
//            print("no modifier keys are pressed")
//        }
    }

    
    static let textBaseAddr = 0x400
    static let textBufferSize = 0x400
    let textLines = 24
    let textCols = 40
    let lineEndChars = 1
    
    var frameCnt = 0
//    let spaceChar : Character = "\u{E17F}"
//    let blockChar : Character = "\u{E07F}"
    let spaceChar : Character = " "
    let blockChar : Character = "░"
    var flashingSpace : Character = " "
    
    let ramBufferPointer = UnsafeRawBufferPointer(start: &RAM, count: 64 * 1024)
    let textBufferPointer = UnsafeRawBufferPointer(start: &RAM + textBaseAddr, count: textBufferSize)
    var txtArr = [Character](repeating: " ", count: textBufferSize)
    
    var s = String()
    
    func HexDump() {
        var txt : String = ""
        
        for y in 0...textLines - 1 {
            txt += String(format: "%04X: ", y * 16)
            for x in 0...15 {
                let byte = ramBufferPointer[ y * 16 + x ]
                let chr = String(format: "%02X ", byte)
                txt += chr
            }
            txt += "\n"
        }
        
        DispatchQueue.main.async {
            self.display.stringValue = txt;
            self.speedometer.stringValue = String(format: "%0.3lf MHz", mhz);
        }
    }
    
    func Update() {

        #if SPEEDTEST
        #else
        m6502_Run()
        #endif
        
//        HexDump()
//        return
        
        frameCnt += 1
        if ( frameCnt == 15 ) {
//            flashingSpace = blockChar
            ViewController.charConvTbl = Array( ViewController.charConvStrFlashOn )
        }
        else if ( frameCnt >= 30 ) {
//            flashingSpace = spaceChar
            ViewController.charConvTbl = Array( ViewController.charConvStrFlashOff )
            frameCnt = 0
        }
        
        var txt : String = ""
        
        for y in 0...textLines-1 {
//            let textAddr = textBaseAddr + textLineOfs[y]
            
//            let linePointer = UnsafeMutableRawPointer( mutating: &RAM + textBaseAddr + y * textCols ) //( start: &RAM + 0x400, count: 0x400)
//            let lineStr = String(bytesNoCopy: linePointer, length: textCols, encoding: .ascii, freeWhenDone: false)!
//            txt += lineStr + "\n"

            for x in 0...textCols-1 {
                let byte = textBufferPointer[ textLineOfs[y] + x ]
                let idx = Int(byte);
                let chr = ViewController.charConvTbl[idx]
                // is it a cursor? (slashing space)
//                if ( chr == "blockChar" ) {
//                    chr = flashingSpace
//                }
    //            print("byte \(index): \(chr)")
//                txt = txt + "\(chr)"
                txtArr[ y * (textCols + lineEndChars) + x ] = chr
            }
            

//            for (_, byte) in textBufferPointer.enumerated() {
//                let idx = Int(byte);
//                var chr = ViewController.charConvTbl[idx]
//                // is it a cursor? (slashing space)
//                if ( chr == blockChar ) {
//                    chr = flashingSpace
//                }
//    //            print("byte \(index): \(chr)")
//                txt = txt + "\(chr)"
//            }
            
//            txt = txt + "\n"
            txtArr[ y * (textCols + lineEndChars) + textCols ] = "\n"
        }
//        txtArr[ textLines * (textCols+1) + textCols ] = "\0"
        txt = String(txtArr)

        
        DispatchQueue.main.async {
            self.display.stringValue = txt;
            self.speedometer.stringValue = String(format: "%0.3lf MHz", mhz);
        }
    }

    
//    func FromBuf(ptr: UnsafeMutablePointer<UInt8>, length len: Int) -> String? {
//        // convert the bytes using the UTF8 encoding
//        if let theString = NSString(bytes: ptr, length: len, encoding: NSUTF8StringEncoding) {
//            return theString as String
//        } else {
//            return nil // the bytes aren't valid UTF8
//        }
//    }

    
    let upd = RepeatingTimer(timeInterval: 1/30)
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        NSEvent.addLocalMonitorForEvents(matching: .flagsChanged) {
            self.flagsChanged(with: $0)
            return $0
        }
        NSEvent.addLocalMonitorForEvents(matching: .keyDown) {
            self.keyDown(with: $0)
            return $0
        }
        
        displayField.maximumNumberOfLines = textLines
        displayField.preferredMaxLayoutWidth = displayField.frame.width

//        DispatchQueue.main.asyncAfter(deadline: .now() + 1/30, execute: {
//            self.update()
//        })

//        #if FUNCTIONTEST
//        #else
//        DispatchQueue.global(qos: .background).async {
//            self.update()
//        }
        
        upd.eventHandler = {
            self.Update()
        }
        upd.resume()
//        #endif
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }

}

