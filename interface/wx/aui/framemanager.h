/////////////////////////////////////////////////////////////////////////////
// Name:        aui/aui.h
// Purpose:     interface of wxAuiManager
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


/**
    @todo wxAuiPaneInfo dock direction types used with wxAuiManager.
*/
enum wxAuiManagerDock
{
    wxAUI_DOCK_NONE = 0,
    wxAUI_DOCK_TOP = 1,
    wxAUI_DOCK_RIGHT = 2,
    wxAUI_DOCK_BOTTOM = 3,
    wxAUI_DOCK_LEFT = 4,
    wxAUI_DOCK_CENTER = 5,
    wxAUI_DOCK_CENTRE = wxAUI_DOCK_CENTER
};


/**
    wxAuiManager behaviour and visual effects style flags.
*/
enum wxAuiManagerOption
{
    /// Allow a pane to be undocked to take the form of a wxMiniFrame.
    wxAUI_MGR_ALLOW_FLOATING           = 1 << 0,
    /// Change the color of the title bar of the pane when it is activated.
    wxAUI_MGR_ALLOW_ACTIVE_PANE        = 1 << 1,
    /// Make the pane transparent during its movement.
    wxAUI_MGR_TRANSPARENT_DRAG         = 1 << 2,
    /// The possible location for docking is indicated by a translucent area.
    wxAUI_MGR_TRANSPARENT_HINT         = 1 << 3,
    /// The possible location for docking is indicated by a gradually appearing
    /// partially transparent area.
    wxAUI_MGR_VENETIAN_BLINDS_HINT     = 1 << 4,
    /// The possible location for docking is indicated by a rectangular outline.
    wxAUI_MGR_RECTANGLE_HINT           = 1 << 5,
    /**
        The translucent area where the pane could be docked appears gradually.

        Note that this flag was included in the default flags until wxWidgets
        3.3.0 but this is not the case in the newer versions. If you'd like to
        still show the hint progressively, you need to explicitly add it to
        wxAUI_MGR_DEFAULT.
     */
    wxAUI_MGR_HINT_FADE                = 1 << 6,
    /**
        Style which disabled the fade-in effect for the docking hint when using
        Venetian blinds hint.

        This style is obsolete and doesn't do anything any longer, fade-in
        effect is only enabled when wxAUI_MGR_HINT_FADE is used.
     */
    wxAUI_MGR_NO_VENETIAN_BLINDS_FADE  = 0,
    /// When a docked pane is resized, its content is refreshed in live (instead of moving
    /// the border alone and refreshing the content at the end).
    /// Since wxWidgets 3.3.0 this flag is included in the default flags.
    wxAUI_MGR_LIVE_RESIZE              = 1 << 8,
    /// Default behaviour.
    wxAUI_MGR_DEFAULT = wxAUI_MGR_ALLOW_FLOATING |
                        wxAUI_MGR_TRANSPARENT_HINT |
                        wxAUI_MGR_LIVE_RESIZE
};

/**
    @class wxAuiManager

    wxAuiManager is the central class of the wxAUI class framework.

    wxAuiManager manages the panes associated with it for a particular window,
    using a pane's wxAuiPaneInfo information to determine each pane's docking
    and floating behaviour.

    wxAuiManager uses wxWidgets' sizer mechanism to plan the layout of each
    frame. It uses a replaceable dock art class to do all drawing, so all
    drawing is localized in one area, and may be customized depending on an
    application's specific needs.

    wxAuiManager works as follows: the programmer adds panes to the class,
    or makes changes to existing pane properties (dock position, floating
    state, show state, etc.). To apply these changes, wxAuiManager's
    Update() function is called. This batch processing can be used to avoid
    flicker, by modifying more than one pane at a time, and then "committing"
    all of the changes at once by calling Update().

    Panes can be added using AddPane():

    @code
    wxTextCtrl* text1 = new wxTextCtrl(this, wxID_ANY);
    wxTextCtrl* text2 = new wxTextCtrl(this, wxID_ANY);
    m_mgr.AddPane(text1, wxLEFT, "Pane Caption");
    m_mgr.AddPane(text2, wxBOTTOM, "Pane Caption");
    m_mgr.Update();
    @endcode

    Later on, the positions and other attributes can be modified, e.g. the
    following will float an existing pane in a tool window:

    @code
    m_mgr.GetPane(text1).Float();
    @endcode


    @section auimanager_layers Layers, Rows and Directions, Positions

    Inside wxAUI, the docking layout is figured out by checking several pane
    parameters. Four of these are important for determining where a pane will
    end up:

    @li Direction: Each docked pane has a direction, Top, Bottom, Left, Right,
        or Center. This is fairly self-explanatory. The pane will be placed in
        the location specified by this variable.
    @li Position: More than one pane can be placed inside of a dock. Imagine
        two panes being docked on the left side of a window. One pane can be
        placed over another. In proportionally managed docks, the pane
        position indicates its sequential position, starting with zero. So, in
        our scenario with two panes docked on the left side, the top pane in
        the dock would have position 0, and the second one would occupy
        position 1.
    @li Row: A row can allow for two docks to be placed next to each other.
        One of the most common places for this to happen is in the toolbar.
        Multiple toolbar rows are allowed, the first row being row 0, and the
        second row 1. Rows can also be used on vertically docked panes.
    @li Layer: A layer is akin to an onion. Layer 0 is the very center of the
        managed pane. Thus, if a pane is in layer 0, it will be closest to the
        center window (also sometimes known as the "content window").
        Increasing layers "swallow up" all layers of a lower value. This can
        look very similar to multiple rows, but is different because all panes
        in a lower level yield to panes in higher levels. The best way to
        understand layers is by running the wxAUI sample.

    @beginStyleTable
    @style{wxAUI_MGR_ALLOW_FLOATING}
           Allow a pane to be undocked to take the form of a wxMiniFrame.
    @style{wxAUI_MGR_ALLOW_ACTIVE_PANE}
           Change the color of the title bar of the pane when it is activated.
    @style{wxAUI_MGR_TRANSPARENT_DRAG}
           Make the pane transparent during its movement.
    @style{wxAUI_MGR_TRANSPARENT_HINT}
           The possible location for docking is indicated by a translucent area.
    @style{wxAUI_MGR_VENETIAN_BLINDS_HINT}
           The possible location for docking is indicated by gradually
           appearing partially transparent hint.
    @style{wxAUI_MGR_RECTANGLE_HINT}
           The possible location for docking is indicated by a rectangular
           outline. Note that this flag doesn't work, i.e. doesn't show any
           hint in wxGTK and wxOSX, please use one of the hint flags above
           instead.
    @style{wxAUI_MGR_HINT_FADE}
           The translucent area where the pane could be docked appears gradually.
           Note that this flag is not included in wxAUI_MGR_DEFAULT since
           wxWidgets 3.3.0 any longer.
    @style{wxAUI_MGR_NO_VENETIAN_BLINDS_FADE}
           This style is obsolete and doesn't do anything, it is only defined
           as 0 for compatibility.
    @style{wxAUI_MGR_LIVE_RESIZE}
           When a docked pane is resized, its content is refreshed in live (instead of moving
           the border alone and refreshing the content at the end). Note that
           this flag is included in wxAUI_MGR_DEFAULT and so needs to be
           explicitly turned off if you don't need. Also note that it is
           always enabled in wxGTK3 and wxOSX ports as non-live resizing is not
           implemented in them.
    @style{wxAUI_MGR_DEFAULT}
           Default behaviour, combines ::wxAUI_MGR_ALLOW_FLOATING,
           ::wxAUI_MGR_TRANSPARENT_HINT and ::wxAUI_MGR_LIVE_RESIZE.
    @endStyleTable

    @beginEventEmissionTable{wxAuiManagerEvent}
    @event{EVT_AUI_PANE_BUTTON(func)}
        Triggered when any button is pressed for any docked panes.
    @event{EVT_AUI_PANE_CLOSE(func)}
        Triggered when a docked or floating pane is closed.
    @event{EVT_AUI_PANE_MAXIMIZE(func)}
        Triggered when a pane is maximized.
    @event{EVT_AUI_PANE_RESTORE(func)}
        Triggered when a pane is restored.
    @event{EVT_AUI_PANE_ACTIVATED(func)}
        Triggered when a pane is made 'active'. This event is new since
        wxWidgets 2.9.4.
    @event{EVT_AUI_RENDER(func)}
        This event can be caught to override the default renderer in order to
        custom draw your wxAuiManager window (not recommended).
    @endEventTable

    @library{wxaui}
    @category{aui}

    @see @ref overview_aui, wxAuiNotebook, wxAuiDockArt, wxAuiPaneInfo
*/
class wxAuiManager : public wxEvtHandler
{
public:
    /**
        Constructor.

        @param managedWindow
            Specifies the window which will contain AUI panes. If it is not
            specified here, it must be set later using SetManagedWindow().
        @param flags
            Specifies the frame management behaviour and visual effects
            with the ::wxAuiManagerOption's style flags.
    */
    wxAuiManager(wxWindow* managedWindow = nullptr,
                 unsigned int flags = wxAUI_MGR_DEFAULT);

    /**
        Dtor.
    */
    virtual ~wxAuiManager();

    //@{
    /**
        AddPane() tells the frame manager to start managing a child window.
        There are several versions of this function. The first version allows
        the full spectrum of pane parameter possibilities. The second version is
        used for simpler user interfaces which do not require as much configuration.
        The last version allows a drop position to be specified, which will determine
        where the pane will be added.
    */
    bool AddPane(wxWindow* window, const wxAuiPaneInfo& pane_info);
    bool AddPane(wxWindow* window, int direction = wxLEFT,
                 const wxString& caption = wxEmptyString);
    bool AddPane(wxWindow* window,
                 const wxAuiPaneInfo& pane_info,
                 const wxPoint& drop_pos);
    //@}

    /**
        Returns true if live resize is always used on the current platform.

        If this function returns true, ::wxAUI_MGR_LIVE_RESIZE flag is ignored
        and live resize is always used, whether it's specified or not.

        Currently this is the case for wxOSX and wxGTK3 when using Wayland, as
        live resizing is the only implemented method there. See
        wxClientDC::CanBeUsedForDrawing() for more details.

        @param window The associated window, may be null (this parameter was
            added in wxWidgets 3.3.0)

        @note As of wxWidgets 3.3.0 this function always returns false.

        @since 3.1.4
     */
    static bool AlwaysUsesLiveResize(const wxWindow* window);

    /**
        This function is used by controls to calculate the drop hint rectangle.

        The method first calls DoDrop() to determine the exact position the
        pane would be at were if dropped.

        @param paneWindow The window pointer of the pane being dragged.
        @param pt The mouse position, in client coordinates.
        @param offset Describes the offset that the mouse is from the upper-left
            corner of the item being dragged, 0 by default (since wxWidgets
            3.3.0, this parameter had to be specified in the earlier versions).
        @return The rectangle hint will be returned in screen coordinates if the pane
            would indeed become docked at the specified drop point.
            Otherwise, an empty rectangle is returned.
    */
    wxRect CalculateHintRect(wxWindow* paneWindow,
                             const wxPoint& pt,
                             const wxPoint& offset = wxPoint{0, 0});

    /**
        Check if a key modifier is pressed (actually ::WXK_CONTROL or
        ::WXK_ALT) while dragging the frame to not dock the window.
    */
    virtual bool CanDockPanel(const wxAuiPaneInfo & p);

    /**
        Destroys or hides the given pane depending on its flags.

        @see wxAuiPaneInfo::DestroyOnClose
    */
    void ClosePane(wxAuiPaneInfo& paneInfo);

    /**
        Creates a floating frame in this wxAuiManager with the given parent and
        wxAuiPaneInfo.
    */
    virtual wxAuiFloatingFrame* CreateFloatingFrame(wxWindow* parent, const wxAuiPaneInfo& p);

    /**
        Tells the wxAuiManager to stop managing the pane specified by window.
        The window, if in a floated frame, is reparented to the frame managed
        by wxAuiManager.
    */
    bool DetachPane(wxWindow* window);

    /**
        This function is used by controls to draw the hint window.

        It is rarely called, and is mostly used by controls implementing custom
        pane drag/drop behaviour.

        Calling it is equivalent to calling CalculateHintRect() and
        UpdateHint() with the resulting rectangle.

        @param paneWindow Window passed to CalculateHintRect().
        @param pt Mouse position passed to CalculateHintRect().
        @param offset Offset passed to CalculateHintRect(), 0 by default (since
            wxWidgets 3.3.0, this parameter had to be specified in the earlier
            versions).
    */
    void DrawHintRect(wxWindow* paneWindow,
                      const wxPoint& pt,
                      const wxPoint& offset = wxPoint{0, 0});

    /**
        Returns an array of all panes managed by the frame manager.
    */
    wxAuiPaneInfoArray& GetAllPanes();

    /// @overload
    const wxAuiPaneInfoArray& GetAllPanes() const;

    /**
        Returns the current art provider being used.
        @see wxAuiDockArt.
    */
    wxAuiDockArt* GetArtProvider() const;

    /**
        Returns the current dock constraint values.
        See SetDockSizeConstraint() for more information.
    */
    void GetDockSizeConstraint(double* widthpct, double* heightpct) const;

    /**
        Returns the current ::wxAuiManagerOption's flags.
    */
    unsigned int GetFlags() const;

    /**
        Returns the frame currently being managed by wxAuiManager.
    */
    wxWindow* GetManagedWindow() const;

    /**
        Calling this method will return the wxAuiManager for a given window.
        The @a window parameter should specify any child window or sub-child
        window of the frame or window managed by wxAuiManager.

        The @a window parameter need not be managed by the manager itself, nor does it
        even need to be a child or sub-child of a managed window. It must however
        be inside the window hierarchy underneath the managed window.
    */
    static wxAuiManager* GetManager(wxWindow* window);

    //@{
    /**
        GetPane() is used to lookup a wxAuiPaneInfo object either by window pointer
        or by pane name, which acts as a unique id for a window pane.

        The returned wxAuiPaneInfo object may then be modified to change a pane's
        look, state or position. After one or more modifications to wxAuiPaneInfo,
        wxAuiManager::Update() should be called to commit the changes to the user
        interface. If the lookup failed (meaning the pane could not be found in the
        manager), a call to the returned wxAuiPaneInfo's IsOk() method will return @false.
    */
    wxAuiPaneInfo& GetPane(wxWindow* window);
    wxAuiPaneInfo& GetPane(const wxString& name);
    //@}

    /**
        Returns true if windows are resized live.

        This function combines the check for AlwaysUsesLiveResize() and, for
        the platforms where live resizing is optional, the check for
        wxAUI_MGR_LIVE_RESIZE flag.

        Using this accessor allows to verify whether live resizing is being
        actually used.

        @since 3.1.4
    */
    bool HasLiveResize() const;

    /**
        HideHint() hides any docking hint that may be visible.

        @see UpdateHint()
    */
    virtual void HideHint();

    /**
        This method is used to insert either a previously unmanaged pane window
        into the frame manager, or to insert a currently managed pane somewhere
        else. InsertPane() will push all panes, rows, or docks aside and
        insert the window into the position specified by @a insert_location.

        Because @a insert_location can specify either a pane, dock row, or dock
        layer, the @a insert_level parameter is used to disambiguate this.
        The parameter @a insert_level can take a value of wxAUI_INSERT_PANE,
        wxAUI_INSERT_ROW or wxAUI_INSERT_DOCK.
    */
    bool InsertPane(wxWindow* window,
                    const wxAuiPaneInfo& insert_location,
                    int insert_level = wxAUI_INSERT_PANE);

    /**
        Load the layout information saved by SaveLayout().

        The implementation of wxAuiDeserializer object passed to this function
        should be consistent with that of the serializer used to save the
        layout. See @ref page_samples_aui for an example of using serializer
        saving the layout in XML format and matching deserializer restoring the
        layout from it.

        @since 3.3.0
     */
    void LoadLayout(wxAuiDeserializer& deserializer);

    /**
        LoadPaneInfo() is similar to LoadPerspective, with the exception that it
        only loads information about a single pane.

        This method writes the serialized data into the passed pane. Pointers to
        UI elements are not modified.

        @note This operation also changes the name in the pane information!

        @see LoadPerspective
        @see SavePaneInfo()
        @see SavePerspective
    */
    void LoadPaneInfo(wxString pane_part, wxAuiPaneInfo& pane);

    /**
        Loads a saved perspective.

        This function is used to load layouts previously saved with
        SavePerspective(), use LoadLayout() to load a layout saved with
        SaveLayout().

        A perspective is the layout state of an AUI managed window.

        All currently existing panes that have an object in "perspective"
        with the same name ("equivalent") will receive the layout parameters of the object in
        "perspective". Existing panes that do not have an equivalent in "perspective" remain
        unchanged, objects in "perspective" having no equivalent in the manager are ignored.

        @param perspective Serialized layout information of a perspective (excl. pointers to UI elements).
        @param update      If update is @true, wxAuiManager::Update() is automatically invoked,
                           thus realizing the specified perspective on screen.

        @see LoadPaneInfo
        @see LoadPerspective
        @see SavePerspective
    */
    bool LoadPerspective(const wxString& perspective,
                         bool update = true);

    /**
        Maximize the given pane.
    */
    void MaximizePane(wxAuiPaneInfo& paneInfo);

    /**
        Restore the last state of the given pane.
    */
    void RestorePane(wxAuiPaneInfo& paneInfo);

    /**
        Restore the previously maximized pane.
    */
    void RestoreMaximizedPane();

    /**
        Save the layout information using the provided object.

        This function allows to use a custom @a serializer to save the layout
        information in any format, e.g. @ref page_samples_aui shows how to save
        it in XML format.

        See wxAuiSerializer documentation for more details.

        @since 3.3.0
     */
    void SaveLayout(wxAuiSerializer& serializer) const;

    /**
        SavePaneInfo() is similar to SavePerspective, with the exception that it only
        saves information about a single pane.

        @param pane Pane whose layout parameters should be serialized.
        @return     The serialized layout parameters of the pane are returned within
                    the string. Information about the pointers to UI elements stored
                    in the pane are not serialized.

        @see LoadPaneInfo
        @see LoadPerspective
        @see SavePerspective
    */
    wxString SavePaneInfo(const wxAuiPaneInfo& pane);

    /**
        Saves the entire user interface layout into an encoded wxString, which
        can then be stored by the application (probably using wxConfig).

        @note You may prefer to use SaveLayout() instead of this function for
            more flexibility.

        @see LoadPerspective
        @see LoadPaneInfo
        @see SavePaneInfo
    */
    wxString SavePerspective();

    /**
        Instructs wxAuiManager to use art provider specified by parameter
        @a art_provider for all drawing calls.
        This allows pluggable look-and-feel features. The previous art provider object,
        if any, will be deleted by wxAuiManager.

        @see wxAuiDockArt.
    */
    void SetArtProvider(wxAuiDockArt* art_provider);

    /**
        When a user creates a new dock by dragging a window into a docked position,
        often times the large size of the window will create a dock that is unwieldy
        large. wxAuiManager by default limits the size of any new dock to 1/3 of the
        window size.  For horizontal docks, this would be 1/3 of the window height.
        For vertical docks, 1/3 of the width.

        Calling this function will adjust this constraint value. The numbers must be
        between 0.0 and 1.0.  For instance, calling SetDockSizeContraint with
        0.5, 0.5 will cause new docks to be limited to half of the size of the
        entire managed window.
    */
    void SetDockSizeConstraint(double widthpct, double heightpct);

    /**
        This method is used to specify ::wxAuiManagerOption's flags. @a flags
        specifies options which allow the frame management behaviour to be modified.
    */
    void SetFlags(unsigned int flags);

    /**
        Set the window which is to be managed by wxAuiManager.

        This window will often be a wxFrame but an arbitrary child window can
        also be used.

        Note that wxAuiManager handles many events for the managed window,
        including ::wxEVT_SIZE, so any application-defined handlers for this
        window should take care to call wxEvent::Skip() to let wxAuiManager
        perform its own processing.
    */
    void SetManagedWindow(wxWindow* managedWindow);

    /**
        This function is used to show a hint window at the specified rectangle.

        It can be overridden to customize the hint appearance. When overriding
        it, HideHint() should normally be also overridden as well.

        Do not call this function directly to show the hint, use UpdateHint()
        instead.

        @param rect The area where the hint window should be shown, in screen
            coordinates, or an empty rectangle to hide the window.
    */
    virtual void ShowHint(const wxRect& rect);

    /**
        Mostly used internally to define the drag action parameters.
    */
    void StartPaneDrag(wxWindow* paneWindow, const wxPoint& offset);

    /**
        Dissociate the managed window from the manager.

        This function may be called before the managed frame or window is
        destroyed, but, since wxWidgets 3.1.4, it's unnecessary to call it
        explicitly, as it will be called automatically when this window is
        destroyed, as well as when the manager itself is.
    */
    void UnInit();

    /**
        This method is called after any number of changes are
        made to any of the managed panes. Update() must be invoked after
        AddPane() or InsertPane() are called in order to "realize" or "commit"
        the changes. In addition, any number of changes may be made to
        wxAuiPaneInfo structures (retrieved with wxAuiManager::GetPane), but to
        realize the changes, Update() must be called. This construction allows
        pane flicker to be avoided by updating the whole layout at one time.
    */
    void Update();

    /**
        Show or hide the hint window.

        This function is mostly used internally.

        @param rect The area where the hint window should be shown, in screen
            coordinates, or an empty rectangle to hide the window.

        @since 3.3.0
     */
    void UpdateHint(const wxRect& rect);

protected:

    /**
        ProcessDockResult() is a protected member of the wxAUI layout manager.
        It can be overridden by derived classes to provide custom docking calculations.
    */
    virtual bool ProcessDockResult(wxAuiPaneInfo& target,
                                   const wxAuiPaneInfo& new_pos);
};



/**
    @class wxAuiPaneInfo

    wxAuiPaneInfo is part of the wxAUI class framework.
    See also @ref overview_aui.

    wxAuiPaneInfo specifies all the parameters for a pane.
    These parameters specify where the pane is on the screen, whether it is docked
    or floating, or hidden.
    In addition, these parameters specify the pane's docked position, floating
    position, preferred size, minimum size, caption text among many other parameters.

    @library{wxaui}
    @category{aui}

    @see wxAuiManager, wxAuiDockArt
*/
class wxAuiPaneInfo
{
public:
    wxAuiPaneInfo();

    /**
        Copy constructor.
    */
    wxAuiPaneInfo(const wxAuiPaneInfo& c);

    //@{
    /**
        BestSize() sets the ideal size for the pane. The docking manager will attempt
        to use this size as much as possible when docking or floating the pane.
    */
    wxAuiPaneInfo& BestSize(const wxSize& size);
    wxAuiPaneInfo& BestSize(int x, int y);
    //@}

    /**
        Bottom() sets the pane dock position to the bottom side of the frame. This is
        the same thing as calling Direction(wxAUI_DOCK_BOTTOM).
    */
    wxAuiPaneInfo& Bottom();

    /**
        BottomDockable() indicates whether a pane can be docked at the bottom of the
        frame.
    */
    wxAuiPaneInfo& BottomDockable(bool b = true);

    /**
        Caption() sets the caption of the pane.
    */
    wxAuiPaneInfo& Caption(const wxString& c);

    /**
        CaptionVisible indicates that a pane caption should be visible. If @false, no
        pane caption is drawn.
    */
    wxAuiPaneInfo& CaptionVisible(bool visible = true);

    //@{
    /**
        Center() sets the pane dock position to the left side of the frame.
        The centre pane is the space in the middle after all border panes (left, top,
        right, bottom) are subtracted from the layout.
        This is the same thing as calling Direction(wxAUI_DOCK_CENTRE).
    */
    wxAuiPaneInfo& Centre();
    wxAuiPaneInfo& Center();
    //@}

    //@{
    /**
        CentrePane() specifies that the pane should adopt the default center pane
        settings. Centre panes usually do not have caption bars.
        This function provides an easy way of preparing a pane to be displayed in
        the center dock position.
    */
    wxAuiPaneInfo& CentrePane();
    wxAuiPaneInfo& CenterPane();
    //@}

    /**
        CloseButton() indicates that a close button should be drawn for the pane.
    */
    wxAuiPaneInfo& CloseButton(bool visible = true);

    /**
        DefaultPane() specifies that the pane should adopt the default pane settings.
    */
    wxAuiPaneInfo& DefaultPane();

    /**
        DestroyOnClose() indicates whether a pane should be destroyed when it is closed.
        Normally a pane is simply hidden when the close button is clicked.
        Setting DestroyOnClose to @true will cause the window to be destroyed when
        the user clicks the pane's close button.
    */
    wxAuiPaneInfo& DestroyOnClose(bool b = true);

    /**
        Direction() determines the direction of the docked pane. It is functionally the
        same as calling Left(), Right(), Top() or Bottom(), except that docking direction
        may be specified programmatically via the parameter.
    */
    wxAuiPaneInfo& Direction(int direction);

    /**
        Dock() indicates that a pane should be docked.  It is the opposite of Float().
    */
    wxAuiPaneInfo& Dock();

    /**
        DockFixed() causes the containing dock to have no resize sash.  This is useful
        for creating panes that span the entire width or height of a dock, but should
        not be resizable in the other direction.
    */
    wxAuiPaneInfo& DockFixed(bool b = true);

    /**
        Dockable() specifies whether a frame can be docked or not. It is the same as
        specifying TopDockable(b).BottomDockable(b).LeftDockable(b).RightDockable(b).
    */
    wxAuiPaneInfo& Dockable(bool b = true);

    /**
        Fixed() forces a pane to be fixed size so that it cannot be resized. After
        calling Fixed(), IsFixed() will return @true.
    */
    wxAuiPaneInfo& Fixed();

    /**
        Float() indicates that a pane should be floated.  It is the opposite of Dock().
    */
    wxAuiPaneInfo& Float();

    /**
        Floatable() sets whether the user will be able to undock a pane and turn it
        into a floating window.
    */
    wxAuiPaneInfo& Floatable(bool b = true);

    //@{
    /**
        FloatingPosition() sets the position of the floating pane.
    */
    wxAuiPaneInfo& FloatingPosition(const wxPoint& pos);
    wxAuiPaneInfo& FloatingPosition(int x, int y);
    //@}

    //@{
    /**
        FloatingSize() sets the size of the floating pane.

        FloatingClientSize() has precedence over this, i.e. this size is ignored
        if the floating client size is specified.
    */
    wxAuiPaneInfo& FloatingSize(const wxSize& size);
    wxAuiPaneInfo& FloatingSize(int x, int y);
    //@}

    //@{
    /**
        FloatingClientSize() sets the client size of the floating pane.

        This has precedence over FloatingSize(), i.e. FloatingSize() is ignored
        if this is specified.

        @see wxWindow::SetClientSize

        @since 3.3.1
    */
    wxAuiPaneInfo& FloatingClientSize(const wxSize& size);
    wxAuiPaneInfo& FloatingClientSize(int x, int y);
    //@}

    /**
        Gripper() indicates that a gripper should be drawn for the pane.
    */
    wxAuiPaneInfo& Gripper(bool visible = true);

    /**
        GripperTop() indicates that a gripper should be drawn at the top of the pane.
    */
    wxAuiPaneInfo& GripperTop(bool attop = true);

    /**
        HasBorder() returns @true if the pane displays a border.
    */
    bool HasBorder() const;

    /**
        HasCaption() returns @true if the pane displays a caption.
    */
    bool HasCaption() const;

    /**
        HasCloseButton() returns @true if the pane displays a button to close the pane.
    */
    bool HasCloseButton() const;

    /**
        HasFlag() returns @true if the property specified by flag is active for
        the pane.
    */
    bool HasFlag(int flag) const;

    /**
        HasGripper() returns @true if the pane displays a gripper.
    */
    bool HasGripper() const;

    /**
        HasGripper() returns @true if the pane displays a gripper at the top.
    */
    bool HasGripperTop() const;

    /**
        HasMaximizeButton() returns @true if the pane displays a button to maximize the
        pane.
    */
    bool HasMaximizeButton() const;

    /**
        HasMinimizeButton() returns @true if the pane displays a button to minimize the
        pane.
    */
    bool HasMinimizeButton() const;

    /**
        HasPinButton() returns @true if the pane displays a button to float the pane.
    */
    bool HasPinButton() const;

    /**
        Hide() indicates that a pane should be hidden.
    */
    wxAuiPaneInfo& Hide();

    /**
        Icon() sets the icon of the pane.

        Notice that the height of the icon should be smaller than the value
        returned by wxAuiDockArt::GetMetric(wxAUI_DOCKART_CAPTION_SIZE) to
        ensure that it appears correctly.

        @since 2.9.2
    */
    wxAuiPaneInfo& Icon(const wxBitmapBundle& b);

    /**
        IsBottomDockable() returns @true if the pane can be docked at the bottom of the
        managed frame.

        @see IsDockable()
    */
    bool IsBottomDockable() const;

    /**
        Returns @true if the pane can be docked at any side.

        @see IsTopDockable(), IsBottomDockable(), IsLeftDockable(), IsRightDockable()

        @since 2.9.2
    */
    bool IsDockable() const;

    /**
        IsDocked() returns @true if the pane is currently docked.
    */
    bool IsDocked() const;

    /**
        IsFixed() returns @true if the pane cannot be resized.
    */
    bool IsFixed() const;

    /**
        IsFloatable() returns @true if the pane can be undocked and displayed as a
        floating window.
    */
    bool IsFloatable() const;

    /**
        IsFloating() returns @true if the pane is floating.
    */
    bool IsFloating() const;

    /**
        IsLeftDockable() returns @true if the pane can be docked on the left of the
        managed frame.

        @see IsDockable()
    */
    bool IsLeftDockable() const;

    /**
        IsMoveable() returns @true if the docked frame can be undocked or moved to
        another dock position.
    */
    bool IsMovable() const;

    /**
        IsOk() returns @true if the wxAuiPaneInfo structure is valid. A pane structure
        is valid if it has an associated window.
    */
    bool IsOk() const;

    /**
        IsResizable() returns @true if the pane can be resized.
    */
    bool IsResizable() const;

    /**
        IsRightDockable() returns @true if the pane can be docked on the right of the
        managed frame.

        @see IsDockable()
    */
    bool IsRightDockable() const;

    /**
        IsShown() returns @true if the pane is currently shown.
    */
    bool IsShown() const;

    /**
        IsToolbar() returns @true if the pane contains a toolbar.
    */
    bool IsToolbar() const;

    /**
        IsTopDockable() returns @true if the pane can be docked at the top of the
        managed frame.

        @see IsDockable()
    */
    bool IsTopDockable() const;

    /**
        Layer() determines the layer of the docked pane. The dock layer is similar to
        an onion, the inner-most layer being layer 0. Each shell moving in the outward
        direction has a higher layer number. This allows for more complex docking layout
        formation.
    */
    wxAuiPaneInfo& Layer(int layer);

    /**
        Left() sets the pane dock position to the left side of the frame. This is the
        same thing as calling Direction(wxAUI_DOCK_LEFT).
    */
    wxAuiPaneInfo& Left();

    /**
        LeftDockable() indicates whether a pane can be docked on the left of the frame.
    */
    wxAuiPaneInfo& LeftDockable(bool b = true);

    //@{
    /**
        MaxSize() sets the maximum size of the pane.
    */
    wxAuiPaneInfo& MaxSize(const wxSize& size);
    wxAuiPaneInfo& MaxSize(int x, int y);
    //@}

    /**
        MaximizeButton() indicates that a maximize button should be drawn for the pane.
    */
    wxAuiPaneInfo& MaximizeButton(bool visible = true);

    //@{
    /**
        MinSize() sets the minimum size of the pane. Please note that this is only
        partially supported as of this writing.
    */
    wxAuiPaneInfo& MinSize(const wxSize& size);
    wxAuiPaneInfo& MinSize(int x, int y);
    //@}

    /**
        MinimizeButton() indicates that a minimize button should be drawn for the pane.
    */
    wxAuiPaneInfo& MinimizeButton(bool visible = true);

    /**
        Movable indicates whether a frame can be moved.
    */
    wxAuiPaneInfo& Movable(bool b = true);

    /**
        Name() sets the name of the pane so it can be referenced in lookup functions.
        If a name is not specified by the user, a random name is assigned to the pane
        when it is added to the manager.
    */
    wxAuiPaneInfo& Name(const wxString& n);

    /**
        PaneBorder indicates that a border should be drawn for the pane.
    */
    wxAuiPaneInfo& PaneBorder(bool visible = true);

    /**
        PinButton() indicates that a pin button should be drawn for the pane.
    */
    wxAuiPaneInfo& PinButton(bool visible = true);

    /**
        Position() determines the position of the docked pane.
    */
    wxAuiPaneInfo& Position(int pos);

    /**
        Resizable() allows a pane to be resized if the parameter is @true, and forces it
        to be a fixed size if the parameter is @false. This is simply an antonym for Fixed().
    */
    wxAuiPaneInfo& Resizable(bool resizable = true);

    /**
        Right() sets the pane dock position to the right side of the frame.
        This is the same thing as calling Direction(wxAUI_DOCK_RIGHT).
    */
    wxAuiPaneInfo& Right();

    /**
        RightDockable() indicates whether a pane can be docked on the right of the
        frame.
    */
    wxAuiPaneInfo& RightDockable(bool b = true);

    /**
        Row() determines the row of the docked pane.
    */
    wxAuiPaneInfo& Row(int row);

    /**
        Write the safe parts of a PaneInfo object "source" into "this".
        "Safe parts" are all non-UI elements (e.g. all layout determining parameters like the
        size, position etc.). "Unsafe parts" (pointers to button, frame and window) are not
        modified by this write operation.

        @remark This method is used when loading perspectives.
    */
    void SafeSet(wxAuiPaneInfo source);

    /**
        SetFlag() turns the property given by flag on or off with the option_state
        parameter.
    */
    wxAuiPaneInfo& SetFlag(int flag, bool option_state);

    /**
        Show() indicates that a pane should be shown.
    */
    wxAuiPaneInfo& Show(bool show = true);

    /**
        ToolbarPane() specifies that the pane should adopt the default toolbar pane
        settings.
    */
    wxAuiPaneInfo& ToolbarPane();

    /**
        Top() sets the pane dock position to the top of the frame.
        This is the same thing as calling Direction(wxAUI_DOCK_TOP).
    */
    wxAuiPaneInfo& Top();

    /**
        TopDockable() indicates whether a pane can be docked at the top of the frame.
    */
    wxAuiPaneInfo& TopDockable(bool b = true);

    /**
        Window() assigns the window pointer that the wxAuiPaneInfo should use.
        This normally does not need to be specified, as the window pointer is
        automatically assigned to the wxAuiPaneInfo structure as soon as it is added
        to the manager.
    */
    wxAuiPaneInfo& Window(wxWindow* w);

    /**
        Makes a copy of the wxAuiPaneInfo object.
    */
    wxAuiPaneInfo& operator=(const wxAuiPaneInfo& c);


    /// name of the pane
    wxString name;

    /// caption displayed on the window
    wxString caption;

    /// icon of the pane, may be invalid
    wxBitmapBundle icon;

    /// window that is in this pane
    wxWindow* window;

    /// floating frame window that holds the pane
    wxFrame* frame;

    /// a combination of wxPaneState values
    unsigned int state;

    /// dock direction (top, bottom, left, right, center)
    int dock_direction;

    /// layer number (0 = innermost layer)
    int dock_layer;

    /// row number on the docking bar (0 = first row)
    int dock_row;

    /// position inside the row (0 = first position)
    int dock_pos;

    /// size that the layout engine will prefer
    wxSize best_size;

    /// minimum size the pane window can tolerate
    wxSize min_size;

    /// maximum size the pane window can tolerate
    wxSize max_size;

    /// position while floating
    wxPoint floating_pos;

    /// size while floating
    wxSize floating_size;

    /// proportion while docked
    int dock_proportion;

    /// current rectangle (populated by wxAUI)
    wxRect rect;

    bool IsValid() const;
};



/**
    @class wxAuiManagerEvent

    Event used to indicate various actions taken with wxAuiManager.

    See wxAuiManager for available event types.

    @beginEventTable{wxAuiManagerEvent}
    @event{EVT_AUI_PANE_BUTTON(func)}
        Triggered when any button is pressed for any docked panes.
    @event{EVT_AUI_PANE_CLOSE(func)}
        Triggered when a docked or floating pane is closed.
    @event{EVT_AUI_PANE_MAXIMIZE(func)}
        Triggered when a pane is maximized.
    @event{EVT_AUI_PANE_RESTORE(func)}
        Triggered when a pane is restored.
    @event{EVT_AUI_PANE_ACTIVATED(func)}
        Triggered when a pane is made 'active'. This event is new since
        wxWidgets 2.9.4.
    @event{EVT_AUI_RENDER(func)}
        This event can be caught to override the default renderer in order to
        custom draw your wxAuiManager window (not recommended).
    @endEventTable

    @library{wxaui}
    @category{events,aui}

    @see wxAuiManager, wxAuiPaneInfo
*/
class wxAuiManagerEvent : public wxEvent
{
public:
    /**
        Constructor.
    */
    wxAuiManagerEvent(wxEventType type = wxEVT_NULL);

    /**
        @return @true if this event can be vetoed.

        @see Veto()
    */
    bool CanVeto();

    /**
        @return The ID of the button that was clicked.
    */
    int GetButton();

    /**
        @todo What is this?
    */
    wxDC* GetDC();

    /**
        @return @true if this event was vetoed.

        @see Veto()
    */
    bool GetVeto();

    /**
        @return The wxAuiManager this event is associated with.
    */
    wxAuiManager* GetManager();

    /**
        @return The pane this event is associated with.
    */
    wxAuiPaneInfo* GetPane();

    /**
        Sets the ID of the button clicked that triggered this event.
    */
    void SetButton(int button);

    /**
        Sets whether or not this event can be vetoed.
    */
    void SetCanVeto(bool can_veto);

    /**
        @todo What is this?
    */
    void SetDC(wxDC* pdc);

    /**
        Sets the wxAuiManager this event is associated with.
    */
    void SetManager(wxAuiManager* manager);

    /**
        Sets the pane this event is associated with.
    */
    void SetPane(wxAuiPaneInfo* pane);

    /**
        Cancels the action indicated by this event if CanVeto() is @true.
    */
    void Veto(bool veto = true);
};



wxEventType wxEVT_AUI_PANE_BUTTON;
wxEventType wxEVT_AUI_PANE_CLOSE;
wxEventType wxEVT_AUI_PANE_MAXIMIZE;
wxEventType wxEVT_AUI_PANE_RESTORE;
wxEventType wxEVT_AUI_PANE_ACTIVATED;
wxEventType wxEVT_AUI_RENDER;
wxEventType wxEVT_AUI_FIND_MANAGER;



class wxAuiDockInfo
{
public:
    wxAuiDockInfo();
    wxAuiDockInfo(const wxAuiDockInfo& c);
    wxAuiDockInfo& operator=(const wxAuiDockInfo& c);

    bool IsOk() const;
    bool IsHorizontal() const;
    bool IsVertical() const;

    wxAuiPaneInfoPtrArray panes; // array of panes
    wxRect rect;              // current rectangle
    int dock_direction;       // dock direction (top, bottom, left, right, center)
    int dock_layer;           // layer number (0 = innermost layer)
    int dock_row;             // row number on the docking bar (0 = first row)
    int size;                 // size of the dock
    int min_size;             // minimum size of a dock (0 if there is no min)
    bool resizable;           // flag indicating whether the dock is resizable
    bool toolbar;             // flag indicating dock contains only toolbars
    bool fixed;               // flag indicating that the dock operates on
                              // absolute coordinates as opposed to proportional
    bool reserved1;
};


class wxAuiDockUIPart
{
public:
    enum
    {
        typeCaption,
        typeGripper,
        typeDock,
        typeDockSizer,
        typePane,
        typePaneSizer,
        typeBackground,
        typePaneBorder,
        typePaneButton
    };

    int type;                // ui part type (see enum above)
    int orientation;         // orientation (either wxHORIZONTAL or wxVERTICAL)
    wxAuiDockInfo* dock;        // which dock the item is associated with
    wxAuiPaneInfo* pane;        // which pane the item is associated with
    int button;              // which pane button the item is associated with
    wxSizer* cont_sizer;     // the part's containing sizer
    wxSizerItem* sizer_item; // the sizer item of the part
    wxRect rect;             // client coord rectangle of the part itself
};
