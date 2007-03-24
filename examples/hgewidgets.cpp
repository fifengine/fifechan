/**
 * HGE widgets example for Guichan. 
 *
 * @author Kevin Lynx
 * @author Olof Naessén
 */

// Include all necessary headers.
#include <guichan.hpp>
#include <guichan/hge.hpp>
#include <hge.h>

/*
 * Common stuff we need
 */
HGE *hge = NULL;
bool running = false;

/*
 * Guichan HGE stuff we need
 */
gnc::HGEInput *input;                    // Input driver
gnc::HGEGraphics *graphics;              // Graphics driver
gnc::HGEImageLoader *imageLoader;        // For loading images
gnc::HGEImageFont *font;                 // For displaying a font

/*
 * Guichan stuff we need
 */
gcn::Gui *gui;                      // A Gui object - binds it all together

/*
 * All of the default widgets
 */
gcn::Container *top;                 // A top container
gcn::Label* label;                   // A label
gcn::Icon* icon;                     // An icon (image)
gcn::Button* button;                 // A button
gcn::TextField* textField;           // One-line text field
gcn::TextBox* textBox;               // Multi-line text box
gcn::ScrollArea* textBoxScrollArea;  // Scroll area for the text box
gcn::ListBox* listBox;               // A list box
gcn::DropDown* dropDown;             // Drop down
gcn::CheckBox* checkBox1;            // Two checkboxes
gcn::CheckBox* checkBox2;
gcn::RadioButton* radioButton1;      // Three radio buttons
gcn::RadioButton* radioButton2;
gcn::RadioButton* radioButton3;
gcn::Slider* slider;                 // A slider
gcn::Image *image;                   // An image for the icon
gcn::Window *window;
gcn::Image *darkbitsImage;
gcn::Icon* darkbitsIcon;
gcn::ScrollArea* nestedScrollArea;
gcn::Container* nestedContainer;
gcn::Slider* nestedSlider;

/*
 * Forward declations
 */
void  init();
void  halt();
bool  frameFunc();

/*
 * List boxes and dropdowns needs an instance of a listmodel
 * to know what elements they have.
 */
class DemoListModel : public gcn::ListModel
{
public:
    int getNumberOfElements()
    {
        return 5;
    }

    std::string getElementAt(int i)
    {
        switch(i)
        {
          case 0:
              return std::string("zero");
          case 1:
              return std::string("one");
          case 2:
              return std::string("two");
          case 3:
              return std::string("three");
          case 4:
              return std::string("four");
          default: // Just to keep warnings away
              return std::string("");
        }  
    }
};

DemoListModel demoListModel;

void initWidgets()
{
    /*
     * Create all the widgets
     */
    label = new gcn::Label("Label");

    image = gcn::Image::load("gui-chan.png");
    icon = new gcn::Icon(image);

    button = new gcn::Button("Button");

    textField = new gcn::TextField("Text field");
  
    textBox = new gcn::TextBox("Multiline\nText box");
    
    textBoxScrollArea = new gcn::ScrollArea(textBox);
    textBoxScrollArea->setWidth(200);
    textBoxScrollArea->setHeight(100);
    textBoxScrollArea->setBorderSize(1);
  
    listBox = new gcn::ListBox(&demoListModel);
    listBox->setBorderSize(1);
  
    dropDown = new gcn::DropDown(&demoListModel);
      
    checkBox1 = new gcn::CheckBox("Checkbox 1");
    checkBox2 = new gcn::CheckBox("Checkbox 2");
  
    radioButton1 = new gcn::RadioButton("RadioButton 1", "radiogroup", true);
    radioButton2 = new gcn::RadioButton("RadioButton 2", "radiogroup");
    radioButton3 = new gcn::RadioButton("RadioButton 3", "radiogroup");
  
    slider = new gcn::Slider(0, 10);
    slider->setSize(100, 10);

    window = new gcn::Window("I am a window  Drag me");
    window->setBaseColor(gcn::Color(255, 150, 200, 190));

    darkbitsImage = gcn::Image::load("darkbitslogo_by_haiko.bmp");
    darkbitsIcon = new gcn::Icon(darkbitsImage);
    window->add(darkbitsIcon);
    window->resizeToContent();

    nestedSlider = new gcn::Slider(0, 10);
    nestedSlider->setSize(100, 10);
    
    nestedContainer = new gcn::Container();
    nestedContainer->setSize(400, 200);
    nestedContainer->add(nestedSlider, 50, 70);

    nestedScrollArea = new gcn::ScrollArea(nestedContainer);
    nestedScrollArea->setSize(180, 90);
    nestedScrollArea->setBorderSize(1);

    /*
     * Add them to the top container
     */
    top->add(label, 10, 10);
    top->add(icon, 10, 30);
    top->add(button, 200, 10);
    top->add(textField, 300, 10);
    top->add(textBoxScrollArea, 200, 50);
    top->add(listBox, 200, 200);
    top->add(dropDown, 500, 10);
    top->add(checkBox1, 500, 130);
    top->add(checkBox2, 500, 150);
    top->add(radioButton1, 500, 200);
    top->add(radioButton2, 500, 220);
    top->add(radioButton3, 500, 240);
    top->add(slider, 500, 300);
    top->add(window, 100, 350);
    top->add(nestedScrollArea, 440, 350);
}

/**
 * Initializes the application
 */
void  init()
{
    imageLoader  = new gcn::HGEImageLoader();
    // The ImageLoader in use is static and must be set to be
    // able to load images
    gcn::Image::setImageLoader(imageLoader);
  
    graphics = new gcn::HGEGraphics();
    input = new gcn::HGEInput();
    
    /*
     * Last but not least it's time to initialize and create the gui
     * with Guichan stuff.
     */
    top = new gcn::Container();
    top->setDimension(gcn::Rectangle(0, 0, 640, 480));
    gui = new gcn::Gui();
    // Set gui to use the HGEGraphics object.
    gui->setGraphics(graphics);
    // Set gui to use the HGEInput object
    gui->setInput(input);
    // Set the top container
    gui->setTop(top);
    // Load the HGE image font.
    font = new gcn::HGEImageFont("font2.fnt");
    // The global font is static and must be set.
    gcn::Widget::setGlobalFont(font);
  
    initWidgets();
}

void  halt()
{
    /*
     * Destroy Guichan stuff
     */
    delete font;
    delete gui;

    /*
     * Widgets
     */
    delete label;
    delete icon;
    delete button;
    delete textField;
    delete textBox;
    delete textBoxScrollArea;
    delete listBox;
    delete dropDown;
    delete checkBox1;
    delete checkBox2;
    delete radioButton1;
    delete radioButton2;
    delete radioButton3;
    delete slider;
    delete window;
    delete darkbitsIcon;
    delete darkbitsImage;
    delete nestedScrollArea;
    delete nestedContainer;
    delete nestedSlider;

    /*
     * Destroy Guichan HGE stuff
     */
    delete input;
    delete graphics;
    delete imageLoader;
}

bool frameFunc()
{
    if (hge->Input_GetKeyState(HGEK_ESCAPE) || running) 
    {
        return true;
    }

    // Let the gui perform it's logic (like handle input)
    gui->logic();
  
    // Begin rendering
    hge->Gfx_BeginScene();
    hge->Gfx_Clear( 0 );
    // Draw the gui
    gui->draw();
    // End rendering
    hge->Gfx_EndScene();
  
    return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    hge = hgeCreate(HGE_VERSION);
  
    hge->System_SetState(HGE_FRAMEFUNC, frameFunc);
  
    hge->System_SetState(HGE_TITLE, "Guichan with HGE Demo : Created By Kevin Lynx 2007-03-16");
    hge->System_SetState(HGE_SCREENWIDTH, 640);
    hge->System_SetState(HGE_SCREENHEIGHT, 480);
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_HIDEMOUSE, false);
    hge->System_SetState(HGE_USESOUND, false);
#ifdef  _DEBUG
    hge->System_SetState(HGE_LOGFILE, "log.txt");
#endif
  
    try
    {
        if (hge->System_Initiate())
        {
            init();
            hge->System_Start();
        }
        else
        { 
            MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        }

        halt();
        hge->System_Shutdown();
        hge->Release();
      
        return 0;
    }
    /*
     * Catch all Guichan exceptions
     */
    catch (gcn::Exception e)
    {
        MessageBox(NULL, hge->System_GetErrorMessage(), e.getMessage(), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    }
    /*
     * Catch all Std exceptions
     */
    catch (std::exception e)
    {
        MessageBox(NULL, hge->System_GetErrorMessage(), e.what(), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    }
    /*
     * Catch all Unknown exceptions
     */
    catch (...)
    {
        MessageBox(NULL, hge->System_GetErrorMessage(), "Unknown exception", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
        return 1;
    }
}

