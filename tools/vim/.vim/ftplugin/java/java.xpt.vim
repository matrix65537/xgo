XPTemplate priority=personal

XPTvar $CDL     /**
XPTvar $CDM     *
XPTvar $CDR     */
XPTvar $CL      //

XPT jcapp   hint=Java\ Card\ Applet
XSET MyApplet|def=fileRoot()
import javacard.framework.*;

public class `MyApplet^ extends Applet`$BRfun^{
    
    protected `MyApplet^`$SPfun^(`$SParg^`$SParg^)`$BRfun^{
        
    }

    public static void install`$SPfun^(`$SParg^byte[] bArray,`$SPop^short bOffset,`$SPop^byte bLength`$SParg^)`$BRfun^{
        new `MyApplet^(`$SParg^`$SParg^).register(`$SParg^bArray,`$SPop^(short)(`$SParg^bOffset`$SPop^+`$SPop^1`$SParg^),`$SPop^bArray[`$SParg^bOffset`$SParg^]`$SParg^);
    }

    public void process`$SPfun^(`$SParg^APDU apdu`$SParg^)`$BRfun^{
        if`$SPcmd^(`$SParg^selectingApplet(`$SParg^`$SParg^)`$SParg^)`$BRif^{
            return;
        }
        byte[] buffer = apdu.getBuffer(`$SParg^`$SParg^);
        switch (`$SParg^buffer[`$SParg^ISO7816.OFFSET_INS`$SParg^]`$SParg^)`$BRif^{
        default:
            ISOException.throwIt(`$SParg^ISO7816.SW_INS_NOT_SUPPORTED`$SParg^);
            break;
        }
    }
}
