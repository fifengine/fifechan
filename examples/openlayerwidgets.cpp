/**
 * OpenLayer widgets example for Guichan.
 */

// Include all necessary headers.
#include <iostream>
#include <guichan.hpp>
#include <guichan/openlayer.hpp>

/*
 * Guichan OpenLayer stuff we need
 */
gcn::OpenLayerInput* input;              // Input driver
gcn::OpenLayerGraphics* graphics;        // Graphics driver
gcn::OpenLayerImageLoader* imageLoader;  // For loading images

/*
 * Guichan stuff we need
 */
gcn::Gui* gui;           // A Gui object - binds it all together
gcn::ImageFont* guiFont; // A font

/*
 * All of the default widgets
 */
gcn::Container* top;                 // A top container
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

    image = gcn::Image::load("gui-chan.bmp");
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
    top->add(textField, 250, 10);
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
void init()
{
    ol::Setup::SetupProgram(true, true, true);
    ol::Setup::SetupScreen(640, 480, WINDOWED);
    
    /*
     * Ok, we have OpenLayer set up, now its time to add the glue
     * between OpenLayer and Guichan
     */
    imageLoader = new gcn::OpenLayerImageLoader();
    
    // The ImageLoader in use is static and must be set to be
    // able to load images
    gcn::Image::setImageLoader(imageLoader);
    
    graphics = new gcn::OpenLayerGraphics();
    graphics->setTargetPlane( 640, 480 );
    
    input = new gcn::OpenLayerInput();
    
    /*
     * Last but not least it's time to initialize and create the gui
     * with Guichan stuff.
     */
    top = new gcn::Container();
    // Set the dimension of the top container to match the screen.
    top->setDimension(gcn::Rectangle(0, 0, 640, 480));
    gui = new gcn::Gui();
    // Set gui to use the OpenLayerGraphics object.
    gui->setGraphics(graphics);
    // Set gui to use the OpenLayerInput object
    gui->setInput(input);
    // Set the top container
    gui->setTop(top);
    // Load the image font.
    guiFont = new gcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    // The global font is static and must be set.
    gcn::Widget::setGlobalFont(guiFont);
    
    initWidgets();
}

/**
 * Halts the application
 */
void halt()
{
    /*
     * Destroy Guichan stuff
     */
    delete guiFont;
    delete gui;
    
    /*
     * Widgets
     */
    delete top;
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
     * Destroy Guichan OpenLayer stuff
     */
    delete input;
    delete graphics;
    delete imageLoader;
    
}

/**
 * Runs the application
 */
void run()
{
    ol::Bitmap mouse_bmp;
    mouse_bmp.Load(mouse_sprite, ol::CONVERT_MAGIC_PINK);
    
    if (!mouse_bmp )
    {
        ol::OlError("Pointer not loaded.");
    }
    
    while(!key[KEY_ESC])
    {
        // Let the gui perform it's logic (like handle input)
        gui->logic();

        // Draw the gui
        gui->draw();
        
        // We draw the mouse pointer manually
        mouse_bmp.Blit(mouse_x, mouse_y);
        
        // Update the screen
        ol::GfxRend::RefreshScreen();
    }
}

int main(int argc, char **argv)
{
    try
    {
        init();
        run();
        halt();
    }
    /*
     * Catch all Guichan exceptions
     */
    catch (gcn::Exception e)
    {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    }
    /*
     * Catch all Std exceptions
     */
    catch (std::exception e)
    {
        std::cerr << "Std exception: " << e.what() << std::endl;
        return 1;
    }
    /*
     * Catch all Unknown exceptions
     */
    catch (...)
    {
        std::cerr << "Unknown exception" << std::endl;
        return 1;
    }
    
    return 0;
}
END_OF_MAIN()
    
    
    
