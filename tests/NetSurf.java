/** @Author: Michael Mullin
 *  @Title: NetSurf
 *  Description: A limited Web-Browser program
 *  Last Modified: April 10th, 2005
 *  Last Modified By: Michael Mullin
 *  
 *  I declare that the attached assignment is my own work 
 *  in accordance with Seneca Academic Policy.  
 *  No part of this assignment has been copied manually or electronically
 *  from any other source (including web sites)
 *  or distributed to other students.
 *  Name                       Student ID
 *  Michael Mullin              029053030
   
*/

package assign3;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.io.Writer;
import javax.swing.event.*;
import javax.swing.text.html.*;
import java.net.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.LinkedList;

public class NetSurf extends JFrame
{
  // the log file
  File f = new File("log.txt");
  FileWriter log = null;
  
  // the serializable file
  private File s = new File("NetSurf.ser");
  Storage netSurfStorage = new Storage();

  // LinkedList for pushing and popping the
  // last viewed page
  // push(aka add) : puts a page into the list
  // for later retrieval by...
  // pop(aka removeLast) : gets the last page
  // saved
  private LinkedList history = new LinkedList();
  
  // initial Page viewing values
  private URL firstView = null;
  private URL homePage = null;

  // menuing objects
  private JMenuBar menu = null;
  private JMenu file = null;
  private JMenu help = null;
  private JMenuItem exit = null;
  private JMenuItem about = null;
  
  // toolbar objects, HOME, BACK, GO
  private JToolBar tBar = null;
  private JButton btnHome = null;
  private JButton btnBack = null;
  private JButton btnGo = null;
  
  // URL input field
  private JTextField urlInput = null;
  private JLabel address = null;
  
  // Main Screen
  private JScrollPane scroll = null;
  private JEditorPane htmlView = null;
  
  // Container
  Container c = null;
  
  /** NetSurf default constructor */
  public NetSurf()
  {
    super("NetSurf");
    
    // address field the user may type in
    urlInput = new JTextField();
    
    // set up the log file
    try
    {
      log = new FileWriter(f,true);
    }catch(IOException ioe)
    {
      // if the logfile cannot be written to
      // we do not want to continue the program
      // because our logging capabilities are gone
      JOptionPane.showMessageDialog(null,"There has been a problem"
        + System.getProperty("line.separator")
        + "I was not able to create or read the log file"
        + System.getProperty("line.separator")
        + "This is a serious Error that I cannot log"
        + System.getProperty("line.separator")
        + "Please check folder permissions"
        + System.getProperty("line.separator")
        + System.getProperty("line.separator")
        + "Program Exiting");
      System.exit(0);
    }
    
    // log startup message
    logAction("STARTUP ---------------------------------------- "
      + System.getProperty("line.separator"),true);
    
    // get current viewing info from the NetSurf.ser file
    FileInputStream serIn = null;
    ObjectInputStream is = null;
    try
    {
      serIn = new FileInputStream(s);
      is = new ObjectInputStream(serIn);
      netSurfStorage = (Storage) is.readObject();
      is.close();
    }catch(FileNotFoundException fnfe) // FileInputStream cannot find file
    {
      logAction("Could Not Find Previous Netsurf.ser File : "
        + "Loading Defaults",true);
      netSurfStorage.setCurrent("http://cs.senecac.on.ca");
      netSurfStorage.setHome("http://cs.senecac.on.ca");
    }catch(IOException ioe) // ObjectInputStream cannot open file
    {
      logAction("NetSurf.ser File Found However, could not read File : "
        + "Loading Defaults",true);
      netSurfStorage.setCurrent("http://cs.senecac.on.ca");
      netSurfStorage.setHome("http://cs.senecac.on.ca");
    }catch(ClassNotFoundException cnfe) // ObjectInputStream cant cast to Storage
    {
      logAction("NetSurf.ser File Found and can be read However, "
        + "Problem with de-serialization : "
        + "Loading Defaults",true);
      netSurfStorage.setCurrent("http://cs.senecac.on.ca");
      netSurfStorage.setHome("http://cs.senecac.on.ca");
    }catch(SecurityException se) // User does not have permission to file
    {
      logAction("You do not have permissions to NetSurf.ser : "
        + "Loading Defaults",true);
    }
    
    // set the last viewed page
    try
    {
      firstView = new URL(netSurfStorage.getCurrent());
      urlInput.setText(netSurfStorage.getCurrent());
    }catch(MalformedURLException murle)
    {
      logAction("StartUp Error : "
        + "Malformed URL : Cannot Set Last Page Visted", true);
      murle.printStackTrace();
    }
    
    // set the homepage
    try
    {
      homePage = new URL(netSurfStorage.getHome());
    }catch(MalformedURLException murle)
    {
      logAction("StartUp Error : "
        + "Malformed URL: Cannot Set Home Page", true);
      murle.printStackTrace();
    }
    
    // get the container ready
    c = getContentPane();
   
    // set up the HTML viewing area
    htmlView = new JEditorPane();
    htmlView.setEditable(false);
    
    // set up the menubar
    menu = new JMenuBar();
    setJMenuBar(menu);
    
    // set up file menu
    file = new JMenu("File");
    file.setMnemonic('f');
    
    // set up help menu
    help = new JMenu("Help");
    help.setMnemonic('h');
    
    // set up file's menu items
    exit = new JMenuItem("Exit");
		exit.setMnemonic('x');

    // set up help's menu items
    about = new JMenuItem("About");
    about.setMnemonic('a');
    
    // add the items to the menues
    file.add(exit);
    help.add(about);
    
    // add the menus to the menubar
    menu.add(file);
    menu.add(help);
    
    // set up the toolbar
    tBar = new JToolBar();
    
    // set up the toolbar buttons and textfield
    btnBack = new JButton("<");
    btnBack.setEnabled(false);
    btnHome = new JButton("HOME");
    btnGo = new JButton("GO");
    address = new JLabel(" Address: ");
    
    // add the buttons to the toolbar
    tBar.add(btnBack);
    tBar.add(btnHome);
    tBar.add(btnGo);
    tBar.add(address);
    tBar.add(urlInput);
    tBar.setEnabled(false);
    
    // add the toolbar to the JFRAME
    c.add(tBar, BorderLayout.NORTH);
    
    // add the JEditorPane to a JScrollPane
    // and then add that to the JFrame
    scroll = new JScrollPane(htmlView);
    c.add(scroll, BorderLayout.CENTER);
    
    // register all the listeners
    registerListeners();
    
    // set the correct size and show the 
    // NetSurf program
    setSize(800,600);
    setVisible(true);
    
    // initial focus should be the address Field
    urlInput.requestFocusInWindow();
    
    
    // set the viewing area to show the First Page
    // which is either the default CS page or
    // the last page visited when NetSurf was last used
    try
    {
      htmlView.setPage(firstView);
    }catch(IOException ioe)
    {
      logAction("StartUp Error : "
        + "Input Output : Cannot Connect to First Page", true);
      protectedURL(firstView); // try authentication
      ioe.printStackTrace();
    }
    
  }
  
  /** a function to handle all of our needed logging capabilities
   *  msg is the message that will be displayed in the logfile
   *  if time is true, then time will be shown in the logfile
   *  if time is false, then the message is intened as an
   *  extention to a previous message
  */
  private void logAction( String msg, boolean time )
  {
    // Date Formatter eg. Thu, Apr 7, 2005: 24:24:51
    SimpleDateFormat sdf = new SimpleDateFormat("EEE, MMM d, yyyy: kk:mm:ss");
    
    try
    {
      // special newline for program exiting
      if (msg.equals("EXIT PROGRAM" + System.getProperty("line.separator")))
        log.write(System.getProperty("line.separator"));
        
      // the time should be added if time is true
      if(time)
        log.write( sdf.format(new Date()) + " : " );
      
      // write the main message
      log.write(msg + System.getProperty("line.separator"));
      // make sure its all in the file
      log.flush();
    }catch(IOException ioe)
    {
      // could not write to the file, so we should print a message
      // in the console
      System.out.println("Could not Record the Log Entry");
      ioe.printStackTrace();
    }
  }

  /** Register Listeners sets up all 
   *  Listeners for our NetSurf program
  */
  private void registerListeners()
  {
  
    /* the hyperlink listener.  This listens to any 
       HTML link clicks inside the JEditorPane
    */
    htmlView.addHyperlinkListener( new HyperlinkListener()
    {
      public void hyperlinkUpdate(HyperlinkEvent hle)
      {
        // make sure its an actual hyperlink click
        if (hle.getEventType() == HyperlinkEvent.EventType.ACTIVATED)
        {
          // store the current page, to be placed into the
          // history list if everything goes well
          URL tmp = htmlView.getPage();
          try
          {
            logAction("HyperLink Action : HyperLink Click : Attempting to Connect to..."
              + System.getProperty("line.separator")
              + " --- " + hle.getURL().toString(), true);
            htmlView.setPage(hle.getURL()); // goto the page
            logAction(" --- Connection Successful", false);
            urlInput.setText(hle.getURL().toString());
            history.add(tmp);
            netSurfStorage.setCurrent(hle.getURL().toString());
            btnBack.setEnabled(true); // we have history to go back to
          } catch (IOException ioe)
          {
            logAction("HyperLink Error : Input Error : Could Not Connect to..."
              + System.getProperty("line.separator")
              + " --- " + hle.getURL().toString(),true);
            protectedURL(hle.getURL()); // try authentication
          }
        }
      }
    });
    /* end of hyperlink listener */
    
    /* menu actions */
    // exit action
    exit.addActionListener(
			new ActionListener() {
				public void actionPerformed(ActionEvent ae) {
					exit();
				}
			}
		);
    
    // about action
	about.addActionListener(
		new ActionListener() {
			public void actionPerformed(ActionEvent ae)
			{
          // show the about message
					JOptionPane.showMessageDialog
						(null, "Author: Michael Mullin"
            + System.getProperty("line.separator")	
            + "Date Last Modified: April 7th, 2005"
            + System.getProperty("line.separator")
            + System.getProperty("line.separator")
            + "Special Features: A Back Button"
            + System.getProperty("line.separator")
            + System.getProperty("line.separator")
            + "Special Thanks: Prof. John Samuel"
            + System.getProperty("line.separator")
            + System.getProperty("line.separator")
            + System.getProperty("line.separator")
            + "TIP: Press the CTRL key to quickly access "
            + "the Address Field");
            
          logAction("Menu About : About MessageBox Shown",true);
				}
			}
		);
    /* end of menu actions */
    
    /* toolbar button actions */
    // back action
    btnBack.addActionListener( new ActionListener() 
    {
      public void actionPerformed(ActionEvent ae) 
      {
        // store the current page to
        // place into our last viewed page
        // if everything goes well
        URL tmp = (URL) history.removeLast();
        try
        {
          logAction("Back Button Press : Attempting to return to..."
            + System.getProperty("line.separator")
            + " --- " + tmp.toString(),true);
          htmlView.setPage(tmp); // go to the page
          netSurfStorage.setCurrent(htmlView.getPage().toString());
          urlInput.setText(tmp.toString());
          logAction(" --- Return Successful",false);
          if (history.isEmpty())
            btnBack.setEnabled(false);
        }catch(IOException ioe)
        {
          logAction("Back Button Error : Could Not connect to..."
            + System.getProperty("line.separator")
            + " --- " + tmp.toString(),true);
          ioe.printStackTrace();
        }
      }
    });
    
    // home action
    btnHome.addActionListener( new ActionListener()
    {
      public void actionPerformed(ActionEvent ae)
      {
        // store the current page to
        // place into our last viewed page
        // if everything goes well
        URL tmp = htmlView.getPage();
        try
        {
          logAction("Home Button Press : Attempting to return to..."
            + System.getProperty("line.separator")
            + " --- " + homePage.toString(),true);
          htmlView.setPage(homePage); // go to the page
          netSurfStorage.setCurrent(homePage.toString());
          urlInput.setText(homePage.toString());
          history.add(tmp);
          btnBack.setEnabled(true);
          logAction(" --- Connection Successful",false);
        }catch(IOException ioe)
        {
          logAction("Home Button Error : Input Error : Could Not connect to..."
            + System.getProperty("line.separator")
            + " --- " + homePage.toString(),true);
          ioe.printStackTrace();
        }
      }
    });
    
    // go action
    btnGo.addActionListener( new ActionListener()
    {
      public void actionPerformed(ActionEvent ae)
      {
        // store the current page to
        // place into our last viewed page
        // if everything goes well
        URL tmp = htmlView.getPage();
        try
        {
          logAction("Go Button Press : Attempting to connect to..."
            + System.getProperty("line.separator")
            + " --- " + proto(urlInput.getText()),true);
          htmlView.setPage(proto(urlInput.getText())); // go to the page
          netSurfStorage.setCurrent(proto(urlInput.getText()));
          history.add(tmp);
          btnBack.setEnabled(true);
          logAction(" --- Connection Successful",false);
        }catch(IOException ioe)
        {
          logAction("Go Button Error : Could Not connect to..."
            + System.getProperty("line.separator")
            + " --- " + proto(urlInput.getText()), true);
          try
          {
            // try authentication
            protectedURL( new URL(proto(urlInput.getText())) );
          }catch(MalformedURLException murle)
            {
              logAction("Go Button Error : Malformed URL "
                + proto(urlInput.getText()),true);
            }
                      
          ioe.printStackTrace();
        }
      }
    });
    
    // key listener for the address bar textfield 
    // listens for when someone types the ENTER key
    // so that the user need not always press the GO button
    urlInput.addKeyListener( new KeyAdapter()
    {
      public void keyPressed( KeyEvent ke )
      {
        if (ke.getKeyChar() == '\n' )
        {
          // store the current page to
          // place into our last viewed page
          // if everything goes well
          URL tmp = htmlView.getPage();
          try
          {
            logAction("Address Field Enter Press : Attempting to connect to..."
              + System.getProperty("line.separator")
              + " --- " + proto(urlInput.getText()),true);
            htmlView.setPage(proto(urlInput.getText())); // go to the page
            netSurfStorage.setCurrent(proto(urlInput.getText()));
            history.add(tmp);
            btnBack.setEnabled(true);
            logAction(" --- Connection Successful",false);
          }catch(IOException ioe)
          {
            logAction("Address Field Enter Press Error : Could Not connect to..."
              + System.getProperty("line.separator")
              + " --- " + proto(urlInput.getText()),true);
            try
            {
              // try authentication
              protectedURL( new URL(proto(urlInput.getText())) );
            }catch(MalformedURLException murle)
            {
              logAction("Address Field Enter Press : Malformed URL "
                + proto(urlInput.getText()),true);
            }
                       
            ioe.printStackTrace();
          }
        }
      }
    });
    /* end of toolbar actions */
    
    /* add quick address field access */
    // keypress on the JEditorPane
    htmlView.addKeyListener( new KeyAdapter()
    {
      public void keyPressed( KeyEvent ke )
      {
        // keyCode 17 is the control key
        if (ke.getKeyCode() == 17)
        {
          logAction("Viewer KeyPress : Focus Given to Address Field",true);  
          urlInput.requestFocusInWindow();
        }
      }
    });
    
    // keypress on the backbutton
    btnBack.addKeyListener( new KeyAdapter()
    {
      public void keyPressed( KeyEvent ke )
      {
        if (ke.getKeyCode() == 17)
        {
          logAction("Back Button KeyPress : Focus Given to Address Field",true);  
          urlInput.requestFocusInWindow();
        }
      }
    });
    
    // keypress on the homebutton
    btnHome.addKeyListener( new KeyAdapter()
    {
      public void keyPressed( KeyEvent ke )
      {
        if (ke.getKeyCode() == 17)
        {
          logAction("Home Button KeyPress : Focus Given to Address Field",true);  
          urlInput.requestFocusInWindow();
        }
      }
    });
    
    // keypress on the go button
    btnGo.addKeyListener( new KeyAdapter()
    {
      public void keyPressed( KeyEvent ke )
      {
        if (ke.getKeyCode() == 17)
        {
          logAction("Go Button KeyPress : Focus Given to Address Field",true);  
          urlInput.requestFocusInWindow();
        }
      }
    });
    
    /* end quick address field access */
  }
  
  /** This is a function that will run if there is authentication
   *  to be done.  If there is no authentication for the page
   *  then the Authenticator.setDefault() does not run
   *  which is why call this when a URL doesn't read
   *  properly from the go button press, the hyperlink press
   *  and the enterkey press from the address field
   *  NOTE: Only authenticates when the URL requests authentication
   *  NOTE: TYPED PASSWORDS ARE VISIBLE
  */
  /* FUTURE FIX: Instead of showInputDialogs, make a custom
   * JFRAME for this specific case
  */
  private void protectedURL( URL inp )
  {
    Authenticator.setDefault(
      new Authenticator()
      {
        protected PasswordAuthentication getPasswordAuthentication()
        {
          logAction(" --- Authentication Attempt", false);  
          String username = null;
          String password = null;
          
          // get the username and password
          // one after the other
          username = (String)
            JOptionPane.showInputDialog(null,"Enter Your UserName");
          password = (String)
            JOptionPane.showInputDialog(null,"WARNING: PASSWORD IS IN CLEARTYPE\n"
              + "Enter Your Password");
          
          // if the user clicked cancel on either the username or
          // password, then dont bother to try authenticating
          // this method keeps running until either
          // the user is authenticated
          // or they give up trying (and thus something is null)
          if( username != null && password != null )
          {
            logAction(" --- Authentication Success", false); 
            return new PasswordAuthentication
              (username, password.toCharArray());
          }else
          {
            logAction(" --- Authentication Failed", false);
            return null;
          }
        }
      }
    );
    try
    {
      htmlView.setPage(inp);
      history.add(inp);
      btnBack.setEnabled(true);
    }catch(IOException ioe)
    {
      logAction("After Authentication : Cannot Connect to..."
        + System.getProperty("line.separator")
        + " --- " + inp.toString(),true);
    }
  }
  
  /** This method makes it so that the user
   *  does not have to always type the http://
  */
  private String proto( String inp )
  {
    if (inp.length() > 5)
    {
      if (!inp.substring(0,7).equals("http://") &&
        !inp.substring(0,6).equals("ftp://") &&
        !inp.substring(0,8).equals("https://"))
      {
        inp = "http://" + inp;
      }
    }else
      inp = "http://" + inp;
    return inp;
  }
  
  /** This method makes sure that
   *  when we exit the program, we write 
   *  the exit message to the logfile
  */
  public void exit()
  {
    FileOutputStream serOut = null;
    ObjectOutputStream os = null;
    try
    {
      serOut = new FileOutputStream(s);
      os = new ObjectOutputStream(serOut);
      os.writeObject(netSurfStorage);  
      os.close();
    }catch(FileNotFoundException fnfe)
    {
      logAction("NetSurf.ser does not exist and I cannot Create it : "
        + "Not Storing home page and current viewing page",true);
    }catch(IOException ioe)
    {
      logAction("NetSurf.ser either exists or can be created However, "
        + "There has been an Output Error : Not Storing home page "
        + "or current viewing page",true);
    }catch(SecurityException se)
    {
      logAction("You do not have permission to store NetSurf.ser : "
        + "Not Storing home page and current viewing page", true);
    }
  
    logAction("EXIT PROGRAM" + System.getProperty("line.separator"),true);
    System.exit(0);
  }
  
  /** MAIN - it all begins here */
  public static void main( String [] args )
  {
    final NetSurf ns = new NetSurf();
    
  	ns.addWindowListener(
      new WindowAdapter()
      {
        public void windowClosing(WindowEvent we)
        {
          ns.exit();
        }
      }
    );
  }
}