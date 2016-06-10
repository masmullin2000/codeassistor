/** title info */

#include <string.h>

#ifndef __KEYWORDS__
#define __KEYWORDS__

#define DESIRED_MB_SIZE 100
#define MAX_FILE_SIZE (DESIRED_MB_SIZE * 0x100000)

enum fileType_t
{
  FILE_TYPE_ERROR = -1,
  FILE_TYPE_UNINIT = 0,
  CFILE,
  CPPFILE,
  OBJCFILE,
  OBJCPPFILE,
  HDFILE,
  JFILE,
  MAX_C_FILETYPES,
  PYFILE,
  PERLFILE,
  PHPFILE,
  TXTFILE,
  SQLFILE,
  MKFILE,
  SHFILE,
  BATFILE,
  MAX_FILE_TYPE
};

#define CKeywords_solo \
  "auto break case char const continue default do double "\
  "else enum extern fload for goto if int long register return short signed " \
  "sizeof static struct switch typedef union unsigned void volatile while "

/* note the words from bool to wchar_t are ANSI-C++ additions */
#define CppKeywords_solo \
  "asm catch class delete friend inline new operator " \
  "private protected public template this throw try virtual " \
  /* ANSI-C++ additions */ \
  "bool const_cast dynamic_cast explicit export false mutable " \
  "namespace reinterpret_cast static_cast true typeid typename using wchar_t "

#define ObjCKeywords_solo \
  "@interface @implementation @protocol @end " \
 "@private @protected @public @try @catch @finally " \
 "@class @selector @protocol @encode @synchronized " \
 "in out inout bycopy byref oneway " \
 "alloc retain release autorelease " \
 "super self "

#define JavaKeywords_solo \
  "abstract assert boolean break byte case catch " \
  "char class const continue default do double else enum extends final " \
  "finally float for goto if implements import instanceof int interface long " \
  "native new package private protected public return short static strictfp " \
  "super switch synchronized this throw throws transient try void volatile while"

#define CStdLibrary_solo \
  /* doesn't include: complex.h fenv.h strbool.h */ \
  /*                  stdint.h tgmath wchar or wctype.h */ \
  /* assert.h */ \
  "assert " \
  /* ctype.h */ \
  "isalnum isalpha iscntrl isdigit isgraph islower " \
  "isprint ispunct isspace isupper isxdigit tolower toupper " \
  /* errno.h */ \
  "errno " \
  /* float.h */ \
  "FLT_RADIX  FLT_MANT_DIG  DBL_MANT_DIG  LDBL_MANT_DIG  FLT_DIG " \
  "DBL_DIG LDBL_DIG  FLT_MIN_EXP  DBL_MIN_EXP  LDBL_MIN_EXP " \
  "FLT_MIN_10_EXP  DBL_MIN_10_EXP  LDBL_MIN_10_EXP " \
  "FLT_MAX_EXP  DBL_MAX_EXP  LDBL_MAX_EXP " \
  "FLT_MAX_10_EXP  DBL_MAX_10_EXP  LDBL_MAX_10_EXP " \
  "FLT_MAX DBL_MAX  LDBL_MAX  FLT_EPSILON  DBL_EPSILON " \
  "LDBL_EPSILON  FLT_MIN DBL_MIN  LDBL_MIN " \
  /* limits.h */ \
  "CHAR_BIT  SCHAR_MIN  SCHAR_MAX  UCHAR_MAX  CHAR_MIN  CHAR_MAX " \
  "MB_LEN_MAX  SHRT_MIN  SHRT_MAX  USHRT_MAX  INT_MIN  INT_MAX " \
  "UINT_MAX  LONG_MIN  LONG_MAX  ULONG_MAX " \
  /* locale.h */ \
  "setlocale localeconv lconv " \
  /* math.h */ \
  "cos sin tan acos asin atan atan2 cosh sinh tanh exp frexp ldexp " \
  "log log10 modf pow sqrt ceil fabs floor fmod " \
  /* setjmp.h */ \
  "longjmp setjmp jmp_buf " \
  /* signal.h */ \
  "signal raise sig_atomic_t " \
  /* stdarg.h */ \
  "va_list va_start va_arg va_end " \
  /* stddef.h */ \
  "ptrdiff_t size_t offsetof NULL " \
  /* stdio.h */ \
  "remove rename tmpfile tmpnam fclose fflush fopen freopen setbuf " \
  "setvbuf fprintf fscanf printf scanf sprintf sscanf vfprintf vprintf " \
  "vsprintf fgetc fgets fputc fputs getc getchar gets putc putchar puts " \
  "ungetc fread fwrite fgetpos fseek fsetpos ftell rewind clearerr " \
  "feof ferror perror EOF FILENAME_MAX TMP_MAX FILE fpos_t " \
  /* stdlib.h */ \
  "atof atoi atol strtod strtol strtoul rand srand calloc free malloc " \
  "realloc abort atexit exit getenv system bsearch qsort abs div labs ldiv " \
  "mblen mbtowc wctomb mbstowcs wcstombs EXIT_FAILURE EXIT_SUCCESS " \
  "MB_CURR_MAX RAND_MAX div_t ldiv_t " \
  /* string.h */ \
  "memcpy memmove strcpy strncpy strcat strncat memcmp strcmp strcoll " \
  "strncmp strxfrm memchr strchr strcspn strpbrk strrchr strspn strstr " \
  "strtok memset strerror strlen " \
  /* time.h */ \
  "clock difftime mktime time asctime ctime gmtime localtime strftime " \
  "CLOCLS_PER_SEC clock_t time_t tm " \
  /* others */ \
  "main " \

#define JavaAllClasses_solo \
  "AbstractAction AbstractBorder AbstractButton " \
  "AbstractCellEditor AbstractCollection AbstractColorChooserPanel " \
  "AbstractDocument AbstractDocument.AttributeContext AbstractDocument.Content " \
  "AbstractDocument.ElementEdit AbstractExecutorService AbstractInterruptibleChannel " \
  "AbstractLayoutCache AbstractLayoutCache.NodeDimensions AbstractList AbstractListModel " \
  "AbstractMap AbstractMethodError AbstractPreferences AbstractQueue " \
  "AbstractQueuedSynchronizer AbstractSelectableChannel AbstractSelectionKey " \
  "AbstractSelector AbstractSequentialList AbstractSet AbstractSpinnerModel " \
  "AbstractTableModel AbstractUndoableEdit AbstractWriter AccessControlContext " \
  "AccessControlException AccessController AccessException Accessible AccessibleAction " \
  "AccessibleAttributeSequence AccessibleBundle AccessibleComponent AccessibleContext " \
  "AccessibleEditableText AccessibleExtendedComponent AccessibleExtendedTable " \
  "AccessibleExtendedText AccessibleHyperlink AccessibleHypertext AccessibleIcon " \
  "AccessibleKeyBinding AccessibleObject AccessibleRelation AccessibleRelationSet " \
  "AccessibleResourceBundle AccessibleRole AccessibleSelection AccessibleState " \
  "AccessibleStateSet AccessibleStreamable AccessibleTable AccessibleTableModelChange " \
  "AccessibleText AccessibleTextSequence AccessibleValue AccountException AccountExpiredException " \
  "AccountLockedException AccountNotFoundException Acl AclEntry AclNotFoundException " \
  "Action ActionEvent ActionListener ActionMap ActionMapUIResource Activatable " \
  "ActivateFailedException ActivationDesc ActivationException ActivationGroup " \
  "ActivationGroup_Stub ActivationGroupDesc ActivationGroupDesc.CommandEnvironment " \
  "ActivationGroupID ActivationID ActivationInstantiator ActivationMonitor ActivationSystem " \
  "Activator ACTIVE ActiveEvent ACTIVITY_COMPLETED ACTIVITY_REQUIRED ActivityCompletedException " \
  "ActivityRequiredException AdapterActivator AdapterActivatorOperations AdapterAlreadyExists " \
  "AdapterAlreadyExistsHelper AdapterInactive AdapterInactiveHelper AdapterManagerIdHelper " \
  "AdapterNameHelper AdapterNonExistent AdapterNonExistentHelper AdapterStateHelper " \
  "AddressHelper Adjustable AdjustmentEvent AdjustmentListener Adler32 AffineTransform " \
  "AffineTransformOp AlgorithmParameterGenerator AlgorithmParameterGeneratorSpi " \
  "AlgorithmParameters AlgorithmParameterSpec AlgorithmParametersSpi AllPermission " \
  "AlphaComposite AlreadyBound AlreadyBoundException AlreadyBoundHelper AlreadyBoundHolder " \
  "AlreadyConnectedException AncestorEvent AncestorListener AnnotatedElement Annotation " \
  "Annotation AnnotationFormatError AnnotationTypeMismatchException Any AnyHolder AnySeqHelper " \
  "AnySeqHelper AnySeqHolder AppConfigurationEntry AppConfigurationEntry.LoginModuleControlFlag " \
  "Appendable Applet AppletContext AppletInitializer AppletStub ApplicationException " \
  "Arc2D Arc2D.Double Arc2D.Float Area AreaAveragingScaleFilter ARG_IN ARG_INOUT " \
  "ARG_OUT ArithmeticException Array Array ArrayBlockingQueue ArrayIndexOutOfBoundsException " \
  "ArrayList Arrays ArrayStoreException ArrayType AssertionError AsyncBoxView " \
  "AsynchronousCloseException AtomicBoolean AtomicInteger AtomicIntegerArray " \
  "AtomicIntegerFieldUpdater AtomicLong AtomicLongArray AtomicLongFieldUpdater " \
  "AtomicMarkableReference AtomicReference AtomicReferenceArray AtomicReferenceFieldUpdater " \
  "AtomicStampedReference Attr Attribute Attribute Attribute AttributeChangeNotification " \
  "AttributeChangeNotificationFilter AttributedCharacterIterator AttributedCharacterIterator.Attribute " \
  "AttributedString AttributeException AttributeInUseException AttributeList AttributeList " \
  "AttributeList AttributeListImpl AttributeModificationException AttributeNotFoundException " \
  "Attributes Attributes Attributes Attributes.Name Attributes2 Attributes2Impl " \
  "AttributeSet AttributeSet AttributeSet.CharacterAttribute AttributeSet.ColorAttribute " \
  "AttributeSet.FontAttribute AttributeSet.ParagraphAttribute AttributeSetUtilities " \
  "AttributesImpl AttributeValueExp AudioClip AudioFileFormat AudioFileFormat.Type " \
  "AudioFileReader AudioFileWriter AudioFormat AudioFormat.Encoding AudioInputStream " \
  "AudioPermission AudioSystem AuthenticationException AuthenticationException " \
  "AuthenticationNotSupportedException Authenticator Authenticator.RequestorType " \
  "AuthorizeCallback AuthPermission AuthProvider Autoscroll AWTError AWTEvent " \
  "AWTEventListener AWTEventListenerProxy AWTEventMulticaster AWTException AWTKeyStroke " \
  "AWTPermission BackingStoreException BAD_CONTEXT BAD_INV_ORDER BAD_OPERATION " \
  "BAD_PARAM BAD_POLICY BAD_POLICY_TYPE BAD_POLICY_VALUE BAD_QOS BAD_TYPECODE " \
  "BadAttributeValueExpException BadBinaryOpValueExpException BadKind BadLocationException " \
  "BadPaddingException BadStringOperationException BandCombineOp BandedSampleModel " \
  "BaseRowSet BasicArrowButton BasicAttribute BasicAttributes BasicBorders " \
  "BasicBorders.ButtonBorder BasicBorders.FieldBorder BasicBorders.MarginBorder " \
  "BasicBorders.MenuBarBorder BasicBorders.RadioButtonBorder BasicBorders.RolloverButtonBorder " \
  "BasicBorders.SplitPaneBorder BasicBorders.ToggleButtonBorder BasicButtonListener " \
  "BasicButtonUI BasicCheckBoxMenuItemUI BasicCheckBoxUI BasicColorChooserUI " \
  "BasicComboBoxEditor BasicComboBoxEditor.UIResource BasicComboBoxRenderer " \
  "BasicComboBoxRenderer.UIResource BasicComboBoxUI BasicComboPopup BasicControl " \
  "BasicDesktopIconUI BasicDesktopPaneUI BasicDirectoryModel BasicEditorPaneUI " \
  "BasicFileChooserUI BasicFormattedTextFieldUI BasicGraphicsUtils BasicHTML " \
  "BasicIconFactory BasicInternalFrameTitlePane BasicInternalFrameUI BasicLabelUI " \
  "BasicListUI BasicLookAndFeel BasicMenuBarUI BasicMenuItemUI BasicMenuUI BasicOptionPaneUI " \
  "BasicOptionPaneUI.ButtonAreaLayout BasicPanelUI BasicPasswordFieldUI BasicPermission " \
  "BasicPopupMenuSeparatorUI BasicPopupMenuUI BasicProgressBarUI BasicRadioButtonMenuItemUI " \
  "BasicRadioButtonUI BasicRootPaneUI BasicScrollBarUI BasicScrollPaneUI BasicSeparatorUI " \
  "BasicSliderUI BasicSpinnerUI BasicSplitPaneDivider BasicSplitPaneUI BasicStroke BasicTabbedPaneUI " \
  "BasicTableHeaderUI BasicTableUI BasicTextAreaUI BasicTextFieldUI BasicTextPaneUI BasicTextUI " \
  "BasicTextUI.BasicCaret BasicTextUI.BasicHighlighter BasicToggleButtonUI BasicToolBarSeparatorUI " \
  "BasicToolBarUI BasicToolTipUI BasicTreeUI BasicViewportUI BatchUpdateException BeanContext " \
  "BeanContextChild BeanContextChildComponentProxy BeanContextChildSupport BeanContextContainerProxy " \
  "BeanContextEvent BeanContextMembershipEvent BeanContextMembershipListener BeanContextProxy " \
  "BeanContextServiceAvailableEvent BeanContextServiceProvider BeanContextServiceProviderBeanInfo " \
  "BeanContextServiceRevokedEvent BeanContextServiceRevokedListener BeanContextServices " \
  "BeanContextServicesListener BeanContextServicesSupport BeanContextServicesSupport.BCSSServiceProvider " \
  "BeanContextSupport BeanContextSupport.BCSIterator BeanDescriptor BeanInfo Beans BevelBorder Bidi " \
  "BigDecimal BigInteger BinaryRefAddr BindException Binding Binding BindingHelper BindingHolder " \
  "BindingIterator BindingIteratorHelper BindingIteratorHolder BindingIteratorOperations " \
  "BindingIteratorPOA BindingListHelper BindingListHolder BindingType BindingTypeHelper " \
  "BindingTypeHolder BitSet Blob BlockingQueue BlockView BMPImageWriteParam Book Boolean " \
  "BooleanControl BooleanControl.Type BooleanHolder BooleanSeqHelper BooleanSeqHolder " \
  "Border BorderFactory BorderLayout BorderUIResource BorderUIResource.BevelBorderUIResource " \
  "BorderUIResource.CompoundBorderUIResource BorderUIResource.EmptyBorderUIResource " \
  "BorderUIResource.EtchedBorderUIResource BorderUIResource.LineBorderUIResource " \
  "BorderUIResource.MatteBorderUIResource BorderUIResource.TitledBorderUIResource BoundedRangeModel " \
  "Bounds Bounds Box Box.Filler BoxedValueHelper BoxLayout BoxView BreakIterator " \
  "BrokenBarrierException Buffer BufferCapabilities BufferCapabilities.FlipContents " \
  "BufferedImage BufferedImageFilter BufferedImageOp BufferedInputStream BufferedOutputStream " \
  "BufferedReader BufferedWriter BufferOverflowException BufferStrategy BufferUnderflowException " \
  "Button ButtonGroup ButtonModel ButtonUI Byte ByteArrayInputStream ByteArrayOutputStream " \
  "ByteBuffer ByteChannel ByteHolder ByteLookupTable ByteOrder CachedRowSet CacheRequest " \
  "CacheResponse Calendar Callable CallableStatement Callback CallbackHandler CancelablePrintJob " \
  "CancellationException CancelledKeyException CannotProceed CannotProceedException " \
  "CannotProceedHelper CannotProceedHolder CannotRedoException CannotUndoException " \
  "Canvas CardLayout Caret CaretEvent CaretListener CDATASection CellEditor " \
  "CellEditorListener CellRendererPane Certificate Certificate Certificate " \
  "Certificate.CertificateRep CertificateEncodingException CertificateEncodingException " \
  "CertificateException CertificateException CertificateExpiredException " \
  "CertificateExpiredException CertificateFactory CertificateFactorySpi " \
  "CertificateNotYetValidException CertificateNotYetValidException CertificateParsingException " \
  "CertificateParsingException CertPath CertPath.CertPathRep CertPathBuilder " \
  "CertPathBuilderException CertPathBuilderResult CertPathBuilderSpi CertPathParameters " \
  "CertPathTrustManagerParameters CertPathValidator CertPathValidatorException " \
  "CertPathValidatorResult CertPathValidatorSpi CertSelector CertStore CertStoreException " \
  "CertStoreParameters CertStoreSpi ChangedCharSetException ChangeEvent " \
  "ChangeListener Channel ChannelBinding Channels Character Character.Subset " \
  "Character.UnicodeBlock CharacterCodingException CharacterData CharacterIterator " \
  "CharArrayReader CharArrayWriter CharBuffer CharConversionException " \
  "CharHolder CharSeqHelper CharSeqHolder CharSequence Charset CharsetDecoder " \
  "CharsetEncoder CharsetProvider Checkbox CheckboxGroup CheckboxMenuItem " \
  "CheckedInputStream CheckedOutputStream Checksum Choice ChoiceCallback " \
  "ChoiceFormat Chromaticity Cipher CipherInputStream CipherOutputStream " \
  "CipherSpi Class ClassCastException ClassCircularityError ClassDefinition " \
  "ClassDesc ClassFileTransformer ClassFormatError ClassLoader ClassLoaderRepository " \
  "ClassLoadingMXBean ClassNotFoundException ClientRequestInfo ClientRequestInfoOperations " \
  "ClientRequestInterceptor ClientRequestInterceptorOperations Clip Clipboard " \
  "ClipboardOwner Clob Cloneable CloneNotSupportedException Closeable ClosedByInterruptException " \
  "ClosedChannelException ClosedSelectorException CMMException Codec CodecFactory " \
  "CodecFactoryHelper CodecFactoryOperations CodecOperations CoderMalfunctionError " \
  "CoderResult CODESET_INCOMPATIBLE CodeSets CodeSigner CodeSource CodingErrorAction " \
  "CollationElementIterator CollationKey Collator Collection CollectionCertStoreParameters " \
  "Collections Color ColorChooserComponentFactory ColorChooserUI ColorConvertOp ColorModel " \
  "ColorSelectionModel ColorSpace ColorSupported ColorType ColorUIResource ComboBoxEditor " \
  "ComboBoxModel ComboBoxUI ComboPopup COMM_FAILURE Comment CommunicationException " \
  "Comparable Comparator CompilationMXBean Compiler CompletionService CompletionStatus " \
  "CompletionStatusHelper Component ComponentAdapter ComponentColorModel ComponentEvent " \
  "ComponentIdHelper ComponentInputMap ComponentInputMapUIResource ComponentListener " \
  "ComponentOrientation ComponentSampleModel ComponentUI ComponentView Composite " \
  "CompositeContext CompositeData CompositeDataSupport CompositeName CompositeType " \
  "CompositeView CompoundBorder CompoundControl CompoundControl.Type CompoundEdit " \
  "CompoundName Compression ConcurrentHashMap ConcurrentLinkedQueue ConcurrentMap " \
  "ConcurrentModificationException Condition Configuration ConfigurationException " \
  "ConfirmationCallback ConnectException ConnectException ConnectIOException Connection " \
  "ConnectionEvent ConnectionEventListener ConnectionPendingException ConnectionPoolDataSource " \
  "ConsoleHandler Constructor Container ContainerAdapter ContainerEvent ContainerListener " \
  "ContainerOrderFocusTraversalPolicy ContentHandler ContentHandler ContentHandlerFactory " \
  "ContentModel Context Context ContextList ContextNotEmptyException ContextualRenderedImageFactory " \
  "Control Control Control.Type ControlFactory ControllerEventListener ConvolveOp " \
  "CookieHandler CookieHolder Copies CopiesSupported CopyOnWriteArrayList CopyOnWriteArraySet " \
  "CountDownLatch CounterMonitor CounterMonitorMBean CRC32 CredentialException " \
  "CredentialExpiredException CredentialNotFoundException CRL CRLException CRLSelector " \
  "CropImageFilter CSS CSS.Attribute CTX_RESTRICT_SCOPE CubicCurve2D CubicCurve2D.Double " \
  "CubicCurve2D.Float Currency Current Current Current CurrentHelper CurrentHelper " \
  "CurrentHelper CurrentHolder CurrentOperations CurrentOperations CurrentOperations " \
  "Cursor Customizer CustomMarshal CustomValue CyclicBarrier DATA_CONVERSION DatabaseMetaData " \
  "DataBuffer DataBufferByte DataBufferDouble DataBufferFloat DataBufferInt DataBufferShort " \
  "DataBufferUShort DataFlavor DataFormatException DatagramChannel DatagramPacket DatagramSocket " \
  "DatagramSocketImpl DatagramSocketImplFactory DataInput DataInputStream DataInputStream " \
  "DataLine DataLine.Info DataOutput DataOutputStream DataOutputStream DataSource DataTruncation " \
  "DatatypeConfigurationException DatatypeConstants DatatypeConstants.Field DatatypeFactory " \
  "Date Date DateFormat DateFormat.Field DateFormatSymbols DateFormatter DateTimeAtCompleted " \
  "DateTimeAtCreation DateTimeAtProcessing DateTimeSyntax DebugGraphics DecimalFormat " \
  "DecimalFormatSymbols DeclHandler DefaultBoundedRangeModel DefaultButtonModel DefaultCaret " \
  "DefaultCellEditor DefaultColorSelectionModel DefaultComboBoxModel DefaultDesktopManager " \
  "DefaultEditorKit DefaultEditorKit.BeepAction DefaultEditorKit.CopyAction DefaultEditorKit.CutAction " \
  "DefaultEditorKit.DefaultKeyTypedAction DefaultEditorKit.InsertBreakAction " \
  "DefaultEditorKit.InsertContentAction DefaultEditorKit.InsertTabAction DefaultEditorKit.PasteAction " \
  "DefaultFocusManager DefaultFocusTraversalPolicy DefaultFormatter DefaultFormatterFactory " \
  "DefaultHandler DefaultHandler2 DefaultHighlighter DefaultHighlighter.DefaultHighlightPainter " \
  "DefaultKeyboardFocusManager DefaultListCellRenderer DefaultListCellRenderer.UIResource " \
  "DefaultListModel DefaultListSelectionModel DefaultLoaderRepository DefaultLoaderRepository " \
  "DefaultMenuLayout DefaultMetalTheme DefaultMutableTreeNode DefaultPersistenceDelegate " \
  "DefaultSingleSelectionModel DefaultStyledDocument DefaultStyledDocument.AttributeUndoableEdit " \
  "DefaultStyledDocument.ElementSpec DefaultTableCellRenderer DefaultTableCellRenderer.UIResource " \
  "DefaultTableColumnModel DefaultTableModel DefaultTextUI DefaultTreeCellEditor " \
  "DefaultTreeCellRenderer DefaultTreeModel DefaultTreeSelectionModel DefinitionKind " \
  "DefinitionKindHelper Deflater DeflaterOutputStream Delayed DelayQueue Delegate " \
  "DelegationPermission Deprecated Descriptor DescriptorAccess DescriptorSupport " \
  "DESedeKeySpec DesignMode DESKeySpec DesktopIconUI DesktopManager DesktopPaneUI " \
  "Destination Destroyable DestroyFailedException DGC DHGenParameterSpec DHKey " \
  "DHParameterSpec DHPrivateKey DHPrivateKeySpec DHPublicKey DHPublicKeySpec Dialog " \
  "Dictionary DigestException DigestInputStream DigestOutputStream Dimension " \
  "Dimension2D DimensionUIResource DirContext DirectColorModel DirectoryManager " \
 "DirObjectFactory DirStateFactory DirStateFactory.Result DISCARDING DisplayMode " \
 "DnDConstants Doc DocAttribute DocAttributeSet DocFlavor DocFlavor.BYTE_ARRAY " \
 "DocFlavor.CHAR_ARRAY DocFlavor.INPUT_STREAM DocFlavor.READER DocFlavor.SERVICE_FORMATTED " \
 "DocFlavor.STRING DocFlavor.URL DocPrintJob Document Document DocumentBuilder " \
 "DocumentBuilderFactory Documented DocumentEvent DocumentEvent DocumentEvent.ElementChange " \
 "DocumentEvent.EventType DocumentFilter DocumentFilter.FilterBypass DocumentFragment " \
 "DocumentHandler DocumentListener DocumentName DocumentParser DocumentType DomainCombiner " \
 "DomainManager DomainManagerOperations  DOMConfiguration DOMError DOMErrorHandler " \
 "DOMException DOMImplementation DOMImplementationList DOMImplementationLS " \
 "DOMImplementationRegistry DOMImplementationSource DOMLocator DOMLocator DOMResult " \
 "DOMSource DOMStringList Double DoubleBuffer DoubleHolder DoubleSeqHelper " \
 "DoubleSeqHolder DragGestureEvent DragGestureListener DragGestureRecognizer DragSource " \
 "DragSourceAdapter DragSourceContext DragSourceDragEvent DragSourceDropEvent " \
 "DragSourceEvent DragSourceListener DragSourceMotionListener Driver DriverManager " \
 "DriverPropertyInfo DropTarget DropTarget.DropTargetAutoScroller DropTargetAdapter " \
 "DropTargetContext DropTargetDragEvent DropTargetDropEvent DropTargetEvent " \
 "DropTargetListener DSAKey DSAKeyPairGenerator DSAParameterSpec DSAParams DSAPrivateKey " \
 "DSAPrivateKeySpec DSAPublicKey DSAPublicKeySpec DTD DTDConstants DTDHandler " \
 "DuplicateFormatFlagsException DuplicateName DuplicateNameHelper Duration " \
 "DynamicImplementation DynamicImplementation DynamicMBean DynAny DynAny DynAnyFactory " \
 "DynAnyFactoryHelper DynAnyFactoryOperations DynAnyHelper DynAnyOperations DynAnySeqHelper " \
 "DynArray DynArray DynArrayHelper DynArrayOperations DynEnum DynEnum DynEnumHelper " \
 "DynEnumOperations DynFixed DynFixed DynFixedHelper DynFixedOperations DynSequence " \
 "DynSequence DynSequenceHelper DynSequenceOperations DynStruct DynStruct DynStructHelper " \
 "DynStructOperations DynUnion DynUnion DynUnionHelper DynUnionOperations DynValue " \
 "DynValue DynValueBox DynValueBoxOperations DynValueCommon DynValueCommonOperations " \
 "DynValueHelper DynValueOperations ECField ECFieldF2m ECFieldFp ECGenParameterSpec " \
 "ECKey ECParameterSpec ECPoint ECPrivateKey ECPrivateKeySpec ECPublicKey ECPublicKeySpec " \
 "EditorKit Element Element Element ElementIterator ElementType Ellipse2D Ellipse2D.Double " \
 "Ellipse2D.Float EllipticCurve EmptyBorder EmptyStackException EncodedKeySpec Encoder " \
 "Encoding ENCODING_CDR_ENCAPS EncryptedPrivateKeyInfo Entity Entity EntityReference " \
 "EntityResolver EntityResolver2 Enum EnumConstantNotPresentException EnumControl " \
 "EnumControl.Type Enumeration EnumMap EnumSet EnumSyntax Environment EOFException " \
 "Error ErrorHandler ErrorListener ErrorManager EtchedBorder Event Event EventContext " \
 "EventDirContext EventException EventHandler EventListener EventListener EventListenerList " \
 "EventListenerProxy EventObject EventQueue EventSetDescriptor EventTarget Exception " \
 "ExceptionDetailMessage ExceptionInInitializerError ExceptionList ExceptionListener " \
 "Exchanger ExecutionException Executor ExecutorCompletionService Executors ExecutorService " \
 "ExemptionMechanism ExemptionMechanismException ExemptionMechanismSpi ExpandVetoException " \
 "ExportException Expression ExtendedRequest ExtendedResponse Externalizable " \
 "FactoryConfigurationError FailedLoginException FeatureDescriptor Fidelity Field " \
 "FieldNameHelper FieldNameHelper FieldPosition FieldView File FileCacheImageInputStream " \
 "FileCacheImageOutputStream FileChannel FileChannel.MapMode FileChooserUI FileDescriptor " \
 "FileDialog FileFilter FileHandler FileImageInputStream FileImageOutputStream " \
 "FileInputStream FileLock FileLockInterruptionException FilenameFilter FileNameMap " \
 "FileNotFoundException FileOutputStream FilePermission FileReader FileSystemView " \
 "FileView FileWriter Filter FilteredImageSource FilteredRowSet FilterInputStream " \
 "FilterOutputStream FilterReader FilterWriter Finishings FixedHeightLayoutCache " \
 "FixedHolder FlatteningPathIterator FlavorEvent FlavorException FlavorListener " \
 "FlavorMap FlavorTable Float FloatBuffer FloatControl FloatControl.Type FloatHolder " \
 "FloatSeqHelper FloatSeqHolder FlowLayout FlowView FlowView.FlowStrategy Flushable " \
 "FocusAdapter FocusEvent FocusListener FocusManager FocusTraversalPolicy Font " \
 "FontFormatException FontMetrics FontRenderContext FontUIResource Format Format.Field " \
 "FormatConversionProvider FormatFlagsConversionMismatchException FormatMismatch " \
 "FormatMismatchHelper Formattable FormattableFlags Formatter Formatter.BigDecimalLayoutForm " \
 "FormatterClosedException FormSubmitEvent FormSubmitEvent.MethodType FormView ForwardRequest " \
 "ForwardRequest ForwardRequestHelper ForwardRequestHelper Frame FREE_MEM Future FutureTask " \
 "GapContent GarbageCollectorMXBean GatheringByteChannel GaugeMonitor GaugeMonitorMBean " \
 "GeneralPath GeneralSecurityException GenericArrayType GenericDeclaration " \
 "GenericSignatureFormatError GlyphJustificationInfo GlyphMetrics GlyphVector GlyphView " \
 "GlyphView.GlyphPainter GradientPaint GraphicAttribute Graphics Graphics2D " \
 "GraphicsConfigTemplate GraphicsConfiguration GraphicsDevice GraphicsEnvironment " \
 "GrayFilter GregorianCalendar GridBagConstraints GridBagLayout GridLayout " \
 "Group GSSContext GSSCredential GSSException GSSManager GSSName Guard GuardedObject " \
 "GZIPInputStream GZIPOutputStream Handler HandlerBase HandshakeCompletedEvent " \
 "HandshakeCompletedListener HasControls HashAttributeSet HashDocAttributeSet " \
 "HashMap HashPrintJobAttributeSet HashPrintRequestAttributeSet HashPrintServiceAttributeSet " \
 "HashSet Hashtable HeadlessException HierarchyBoundsAdapter HierarchyBoundsListener " \
 "HierarchyEvent HierarchyListener Highlighter Highlighter.Highlight Highlighter.HighlightPainter " \
 "HOLDING HostnameVerifier HTML HTML.Attribute HTML.Tag HTML.UnknownTag HTMLDocument " \
 "HTMLDocument.Iterator HTMLEditorKit HTMLEditorKit.HTMLFactory HTMLEditorKit.HTMLTextAction " \
 "HTMLEditorKit.InsertHTMLTextAction HTMLEditorKit.LinkController HTMLEditorKit.Parser " \
 "HTMLEditorKit.ParserCallback HTMLFrameHyperlinkEvent HTMLWriter HttpRetryException " \
 "HttpsURLConnection HttpURLConnection HyperlinkEvent HyperlinkEvent.EventType " \
 "HyperlinkListener ICC_ColorSpace ICC_Profile ICC_ProfileGray ICC_ProfileRGB Icon " \
 "IconUIResource IconView ID_ASSIGNMENT_POLICY_ID ID_UNIQUENESS_POLICY_ID IdAssignmentPolicy " \
 "IdAssignmentPolicyOperations IdAssignmentPolicyValue IdentifierHelper Identity " \
 "IdentityHashMap IdentityScope IDLEntity IDLType IDLTypeHelper IDLTypeOperations " \
 "IdUniquenessPolicy IdUniquenessPolicyOperations IdUniquenessPolicyValue IIOByteBuffer " \
 "IIOException IIOImage IIOInvalidTreeException IIOMetadata IIOMetadataController " \
 "IIOMetadataFormat IIOMetadataFormatImpl IIOMetadataNode IIOParam IIOParamController " \
 "IIOReadProgressListener IIOReadUpdateListener IIOReadWarningListener IIORegistry " \
 "IIOServiceProvider IIOWriteProgressListener IIOWriteWarningListener IllegalAccessError " \
 "IllegalAccessException IllegalArgumentException IllegalBlockingModeException " \
 "IllegalBlockSizeException IllegalCharsetNameException IllegalClassFormatException " \
 "IllegalComponentStateException IllegalFormatCodePointException IllegalFormatConversionException " \
 "IllegalFormatException IllegalFormatFlagsException IllegalFormatPrecisionException " \
 "IllegalFormatWidthException IllegalMonitorStateException IllegalPathStateException " \
 "IllegalSelectorException IllegalStateException IllegalThreadStateException Image " \
 "ImageCapabilities ImageConsumer ImageFilter ImageGraphicAttribute ImageIcon " \
 "ImageInputStream ImageInputStreamImpl ImageInputStreamSpi ImageIO ImageObserver " \
 "ImageOutputStream ImageOutputStreamImpl ImageOutputStreamSpi ImageProducer ImageReader " \
 "ImageReaderSpi ImageReaderWriterSpi ImageReadParam ImageTranscoder ImageTranscoderSpi " \
 "ImageTypeSpecifier ImageView ImageWriteParam ImageWriter ImageWriterSpi ImagingOpException " \
 "IMP_LIMIT IMPLICIT_ACTIVATION_POLICY_ID ImplicitActivationPolicy ImplicitActivationPolicyOperations " \
 "ImplicitActivationPolicyValue INACTIVE IncompatibleClassChangeError IncompleteAnnotationException " \
 "InconsistentTypeCode InconsistentTypeCode InconsistentTypeCodeHelper IndexColorModel " \
 "IndexedPropertyChangeEvent IndexedPropertyDescriptor IndexOutOfBoundsException " \
 "IndirectionException Inet4Address Inet6Address InetAddress InetSocketAddress Inflater " \
 "InflaterInputStream InheritableThreadLocal Inherited InitialContext InitialContextFactory " \
 "InitialContextFactoryBuilder InitialDirContext INITIALIZE InitialLdapContext InlineView " \
 "InputContext InputEvent InputMap InputMapUIResource InputMethod InputMethodContext " \
 "InputMethodDescriptor InputMethodEvent InputMethodHighlight InputMethodListener " \
 "InputMethodRequests InputMismatchException InputSource InputStream InputStream InputStream " \
 "InputStreamReader InputSubset InputVerifier Insets InsetsUIResource InstanceAlreadyExistsException " \
 "InstanceNotFoundException InstantiationError InstantiationException Instrument " \
 "Instrumentation InsufficientResourcesException IntBuffer Integer IntegerSyntax " \
 "Interceptor InterceptorOperations INTERNAL InternalError InternalFrameAdapter " \
 "InternalFrameEvent InternalFrameFocusTraversalPolicy InternalFrameListener " \
 "InternalFrameUI InternationalFormatter InterruptedException InterruptedIOException " \
 "InterruptedNamingException InterruptibleChannel INTF_REPOS IntHolder IntrospectionException " \
 "Introspector INV_FLAG INV_IDENT INV_OBJREF INV_POLICY Invalid INVALID_ACTIVITY " \
 "INVALID_TRANSACTION InvalidActivityException InvalidAddress InvalidAddressHelper " \
 "InvalidAddressHolder InvalidAlgorithmParameterException InvalidApplicationException " \
 "InvalidAttributeIdentifierException InvalidAttributesException InvalidAttributeValueException " \
 "InvalidAttributeValueException InvalidClassException InvalidDnDOperationException " \
 "InvalidKeyException InvalidKeyException InvalidKeySpecException InvalidMarkException " \
 "InvalidMidiDataException InvalidName InvalidName InvalidName InvalidNameException " \
 "InvalidNameHelper InvalidNameHelper InvalidNameHolder InvalidObjectException " \
 "InvalidOpenTypeException InvalidParameterException InvalidParameterSpecException " \
 "InvalidPolicy InvalidPolicyHelper InvalidPreferencesFormatException " \
 "InvalidPropertiesFormatException InvalidRelationIdException InvalidRelationServiceException " \
 "InvalidRelationTypeException InvalidRoleInfoException InvalidRoleValueException " \
 "InvalidSearchControlsException InvalidSearchFilterException InvalidSeq InvalidSlot " \
 "InvalidSlotHelper InvalidTargetObjectTypeException InvalidTransactionException " \
 "InvalidTypeForEncoding InvalidTypeForEncodingHelper InvalidValue InvalidValue " \
 "InvalidValueHelper InvocationEvent InvocationHandler InvocationTargetException " \
 "InvokeHandler IOException IOR IORHelper IORHolder IORInfo IORInfoOperations " \
 "IORInterceptor IORInterceptor_3_0 IORInterceptor_3_0Helper IORInterceptor_3_0Holder " \
 "IORInterceptor_3_0Operations IORInterceptorOperations IRObject IRObjectOperations " \
 "IstringHelper ItemEvent ItemListener ItemSelectable Iterable Iterator IvParameterSpec " \
 "JApplet JarEntry JarException JarFile JarInputStream JarOutputStream JarURLConnection " \
 "JButton JCheckBox JCheckBoxMenuItem JColorChooser JComboBox JComboBox.KeySelectionManager " \
 "JComponent JdbcRowSet JDesktopPane JDialog JEditorPane JFileChooser JFormattedTextField " \
 "JFormattedTextField.AbstractFormatter JFormattedTextField.AbstractFormatterFactory " \
 "JFrame JInternalFrame JInternalFrame.JDesktopIcon JLabel JLayeredPane JList JMenu " \
 "JMenuBar JMenuItem JMException JMRuntimeException JMXAuthenticator JMXConnectionNotification " \
 "JMXConnector JMXConnectorFactory JMXConnectorProvider JMXConnectorServer JMXConnectorServerFactory " \
 "JMXConnectorServerMBean JMXConnectorServerProvider JMXPrincipal JMXProviderException " \
 "JMXServerErrorException JMXServiceURL JobAttributes JobAttributes.DefaultSelectionType " \
 "JobAttributes.DestinationType JobAttributes.DialogType JobAttributes.MultipleDocumentHandlingType " \
 "JobAttributes.SidesType JobHoldUntil JobImpressions JobImpressionsCompleted " \
 "JobImpressionsSupported JobKOctets JobKOctetsProcessed JobKOctetsSupported JobMediaSheets " \
 "JobMediaSheetsCompleted JobMediaSheetsSupported JobMessageFromOperator JobName " \
 "JobOriginatingUserName JobPriority JobPrioritySupported JobSheets JobState JobStateReason " \
 "JobStateReasons Joinable JoinRowSet JOptionPane JPanel JPasswordField JPEGHuffmanTable " \
 "JPEGImageReadParam JPEGImageWriteParam JPEGQTable JPopupMenu JPopupMenu.Separator " \
 "JProgressBar JRadioButton JRadioButtonMenuItem JRootPane JScrollBar JScrollPane " \
 "JSeparator JSlider JSpinner JSpinner.DateEditor JSpinner.DefaultEditor JSpinner.ListEditor " \
 "JSpinner.NumberEditor JSplitPane JTabbedPane JTable JTable.PrintMode JTableHeader " \
 "JTextArea JTextComponent JTextComponent.KeyBinding JTextField JTextPane JToggleButton " \
 "JToggleButton.ToggleButtonModel JToolBar JToolBar.Separator JToolTip JTree " \
 "JTree.DynamicUtilTreeNode JTree.EmptySelectionModel JViewport JWindow KerberosKey " \
 "KerberosPrincipal KerberosTicket Kernel Key KeyAdapter KeyAgreement KeyAgreementSpi " \
 "KeyAlreadyExistsException KeyboardFocusManager KeyEvent KeyEventDispatcher " \
 "KeyEventPostProcessor KeyException KeyFactory KeyFactorySpi KeyGenerator KeyGeneratorSpi " \
 "KeyListener KeyManagementException KeyManager KeyManagerFactory KeyManagerFactorySpi " \
 "Keymap KeyPair KeyPairGenerator KeyPairGeneratorSpi KeyRep KeyRep.Type KeySpec " \
 "KeyStore KeyStore.Builder KeyStore.CallbackHandlerProtection KeyStore.Entry " \
 "KeyStore.LoadStoreParameter KeyStore.PasswordProtection KeyStore.PrivateKeyEntry " \
 "KeyStore.ProtectionParameter KeyStore.SecretKeyEntry KeyStore.TrustedCertificateEntry " \
 "KeyStoreBuilderParameters KeyStoreException KeyStoreSpi KeyStroke Label LabelUI " \
 "LabelView LanguageCallback LastOwnerException LayeredHighlighter LayeredHighlighter.LayerPainter " \
 "LayoutFocusTraversalPolicy LayoutManager LayoutManager2 LayoutQueue LDAPCertStoreParameters " \
 "LdapContext LdapName LdapReferralException Lease Level LexicalHandler LIFESPAN_POLICY_ID " \
 "LifespanPolicy LifespanPolicyOperations LifespanPolicyValue LimitExceededException " \
 "Line Line.Info Line2D Line2D.Double Line2D.Float LineBorder LineBreakMeasurer LineEvent " \
 "LineEvent.Type LineListener LineMetrics LineNumberInputStream LineNumberReader " \
 "LineUnavailableException LinkageError LinkedBlockingQueue LinkedHashMap LinkedHashSet " \
 "LinkedList LinkException LinkLoopException LinkRef List List ListCellRenderer " \
 "ListDataEvent ListDataListener ListenerNotFoundException ListIterator ListModel " \
 "ListResourceBundle ListSelectionEvent ListSelectionListener ListSelectionModel " \
 "ListUI ListView LoaderHandler Locale LocalObject LocateRegistry LOCATION_FORWARD " \
 "Locator Locator2 Locator2Impl LocatorImpl Lock LockSupport Logger LoggingMXBean " \
 "LoggingPermission LoginContext LoginException LoginModule LogManager LogRecord " \
 "LogStream Long LongBuffer LongHolder LongLongSeqHelper LongLongSeqHolder LongSeqHelper " \
 "LongSeqHolder LookAndFeel LookupOp LookupTable LSException LSInput LSLoadEvent " \
 "LSOutput LSParser LSParserFilter LSProgressEvent LSResourceResolver LSSerializer " \
 "LSSerializerFilter Mac MacSpi MalformedInputException MalformedLinkException " \
 "MalformedObjectNameException MalformedParameterizedTypeException MalformedURLException " \
 "ManagementFactory ManagementPermission ManageReferralControl ManagerFactoryParameters " \
 "Manifest Map Map.Entry MappedByteBuffer MARSHAL MarshalException MarshalledObject " \
 "MaskFormatter Matcher MatchResult Math MathContext MatteBorder MBeanAttributeInfo " \
 "MBeanConstructorInfo MBeanException MBeanFeatureInfo MBeanInfo MBeanNotificationInfo " \
 "MBeanOperationInfo MBeanParameterInfo MBeanPermission MBeanRegistration " \
 "MBeanRegistrationException MBeanServer MBeanServerBuilder MBeanServerConnection " \
 "MBeanServerDelegate MBeanServerDelegateMBean  MBeanServerFactory MBeanServerForwarder " \
 "MBeanServerInvocationHandler MBeanServerNotification MBeanServerNotificationFilter " \
 "MBeanServerPermission MBeanTrustPermission Media MediaName MediaPrintableArea " \
 "MediaSize MediaSize.Engineering MediaSize.ISO MediaSize.JIS MediaSize.NA " \
 "MediaSize.Other MediaSizeName MediaTracker MediaTray Member MemoryCacheImageInputStream " \
 "MemoryCacheImageOutputStream MemoryHandler MemoryImageSource MemoryManagerMXBean " \
 "MemoryMXBean MemoryNotificationInfo MemoryPoolMXBean MemoryType MemoryUsage Menu " \
 "MenuBar MenuBarUI MenuComponent MenuContainer MenuDragMouseEvent MenuDragMouseListener " \
 "MenuElement MenuEvent MenuItem MenuItemUI MenuKeyEvent MenuKeyListener MenuListener " \
 "MenuSelectionManager MenuShortcut MessageDigest MessageDigestSpi MessageFormat " \
 "MessageFormat.Field MessageProp MetaEventListener MetalBorders MetalBorders.ButtonBorder " \
 "MetalBorders.Flush3DBorder MetalBorders.InternalFrameBorder MetalBorders.MenuBarBorder " \
 "MetalBorders.MenuItemBorder MetalBorders.OptionDialogBorder MetalBorders.PaletteBorder " \
 "MetalBorders.PopupMenuBorder MetalBorders.RolloverButtonBorder MetalBorders.ScrollPaneBorder " \
 "MetalBorders.TableHeaderBorder MetalBorders.TextFieldBorder MetalBorders.ToggleButtonBorder " \
 "MetalBorders.ToolBarBorder MetalButtonUI MetalCheckBoxIcon MetalCheckBoxUI MetalComboBoxButton " \
 "MetalComboBoxEditor MetalComboBoxEditor.UIResource MetalComboBoxIcon MetalComboBoxUI " \
 "MetalDesktopIconUI MetalFileChooserUI MetalIconFactory MetalIconFactory.FileIcon16 " \
 "MetalIconFactory.FolderIcon16 MetalIconFactory.PaletteCloseIcon MetalIconFactory.TreeControlIcon " \
 "MetalIconFactory.TreeFolderIcon MetalIconFactory.TreeLeafIcon MetalInternalFrameTitlePane " \
 "MetalInternalFrameUI MetalLabelUI MetalLookAndFeel MetalMenuBarUI MetalPopupMenuSeparatorUI " \
 "MetalProgressBarUI MetalRadioButtonUI MetalRootPaneUI MetalScrollBarUI MetalScrollButton " \
 "MetalScrollPaneUI MetalSeparatorUI MetalSliderUI MetalSplitPaneUI MetalTabbedPaneUI " \
 "MetalTextFieldUI MetalTheme MetalToggleButtonUI MetalToolBarUI MetalToolTipUI " \
 "MetalTreeUI MetaMessage Method MethodDescriptor MGF1ParameterSpec MidiChannel " \
 "MidiDevice MidiDevice.Info MidiDeviceProvider MidiEvent MidiFileFormat MidiFileReader " \
 "MidiFileWriter MidiMessage MidiSystem MidiUnavailableException MimeTypeParseException " \
 "MinimalHTMLWriter MissingFormatArgumentException MissingFormatWidthException " \
 "MissingResourceException Mixer Mixer.Info MixerProvider MLet MLetMBean ModelMBean " \
 "ModelMBeanAttributeInfo ModelMBeanConstructorInfo ModelMBeanInfo ModelMBeanInfoSupport " \
 "ModelMBeanNotificationBroadcaster ModelMBeanNotificationInfo ModelMBeanOperationInfo " \
 "ModificationItem Modifier Monitor MonitorMBean MonitorNotification MonitorSettingException " \
 "MouseAdapter MouseDragGestureRecognizer MouseEvent MouseEvent MouseInfo MouseInputAdapter " \
 "MouseInputListener MouseListener MouseMotionAdapter MouseMotionListener MouseWheelEvent " \
 "MouseWheelListener MultiButtonUI MulticastSocket MultiColorChooserUI MultiComboBoxUI " \
 "MultiDesktopIconUI MultiDesktopPaneUI MultiDoc MultiDocPrintJob MultiDocPrintService " \
 "MultiFileChooserUI MultiInternalFrameUI MultiLabelUI MultiListUI MultiLookAndFeel " \
 "MultiMenuBarUI MultiMenuItemUI MultiOptionPaneUI MultiPanelUI MultiPixelPackedSampleModel " \
 "MultipleComponentProfileHelper MultipleComponentProfileHolder MultipleDocumentHandling " \
 "MultipleMaster MultiPopupMenuUI MultiProgressBarUI MultiRootPaneUI MultiScrollBarUI " \
 "MultiScrollPaneUI MultiSeparatorUI MultiSliderUI MultiSpinnerUI MultiSplitPaneUI " \
 "MultiTabbedPaneUI MultiTableHeaderUI MultiTableUI MultiTextUI MultiToolBarUI " \
 "MultiToolTipUI MultiTreeUI MultiViewportUI MutableAttributeSet MutableComboBoxModel " \
 "MutableTreeNode MutationEvent Name NameAlreadyBoundException NameCallback " \
 "NameClassPair NameComponent NameComponentHelper NameComponentHolder NamedNodeMap " \
 "NamedValue NameDynAnyPair NameDynAnyPairHelper NameDynAnyPairSeqHelper NameHelper " \
 "NameHolder NameList NameNotFoundException NameParser NamespaceChangeListener " \
 "NamespaceContext NamespaceSupport NameValuePair NameValuePair NameValuePairHelper " \
 "NameValuePairHelper NameValuePairSeqHelper Naming NamingContext NamingContextExt " \
 "NamingContextExtHelper NamingContextExtHolder NamingContextExtOperations NamingContextExtPOA " \
 "NamingContextHelper NamingContextHolder NamingContextOperations NamingContextPOA " \
 "NamingEnumeration NamingEvent NamingException NamingExceptionEvent NamingListener " \
 "NamingManager NamingSecurityException NavigationFilter NavigationFilter.FilterBypass " \
 "NegativeArraySizeException NetPermission NetworkInterface NO_IMPLEMENT NO_MEMORY " \
 "NO_PERMISSION NO_RESOURCES NO_RESPONSE NoClassDefFoundError NoConnectionPendingException " \
 "NoContext NoContextHelper Node NodeChangeEvent NodeChangeListener NodeList " \
 "NoInitialContextException NON_EXISTENT NoninvertibleTransformException " \
 "NonReadableChannelException NonWritableChannelException NoPermissionException " \
 "NoRouteToHostException NoServant NoServantHelper NoSuchAlgorithmException " \
 "NoSuchAttributeException NoSuchElementException NoSuchFieldError NoSuchFieldException " \
 "NoSuchMethodError NoSuchMethodException NoSuchObjectException NoSuchPaddingException " \
 "NoSuchProviderException NotActiveException Notation NotBoundException NotCompliantMBeanException " \
 "NotContextException NotEmpty NotEmptyHelper NotEmptyHolder NotFound NotFoundHelper " \
 "NotFoundHolder NotFoundReason NotFoundReasonHelper NotFoundReasonHolder Notification " \
 "NotificationBroadcaster NotificationBroadcasterSupport NotificationEmitter NotificationFilter " \
 "NotificationFilterSupport NotificationListener NotificationResult NotOwnerException " \
 "NotSerializableException NotYetBoundException NotYetConnectedException NullCipher " \
 "NullPointerException Number NumberFormat NumberFormat.Field NumberFormatException " \
 "NumberFormatter NumberOfDocuments NumberOfInterveningJobs NumberUp NumberUpSupported " \
 "NumericShaper NVList OAEPParameterSpec OBJ_ADAPTER Object Object OBJECT_NOT_EXIST " \
 "ObjectAlreadyActive ObjectAlreadyActiveHelper ObjectChangeListener ObjectFactory " \
 "ObjectFactoryBuilder ObjectHelper ObjectHolder ObjectIdHelper ObjectIdHelper " \
 "ObjectImpl ObjectImpl ObjectInput ObjectInputStream ObjectInputStream.GetField " \
 "ObjectInputValidation ObjectInstance ObjectName ObjectNotActive ObjectNotActiveHelper " \
 "ObjectOutput ObjectOutputStream ObjectOutputStream.PutField ObjectReferenceFactory " \
 "ObjectReferenceFactoryHelper ObjectReferenceFactoryHolder ObjectReferenceTemplate " \
 "ObjectReferenceTemplateHelper ObjectReferenceTemplateHolder ObjectReferenceTemplateSeqHelper " \
 "ObjectReferenceTemplateSeqHolder ObjectStreamClass ObjectStreamConstants " \
 "ObjectStreamException ObjectStreamField ObjectView ObjID Observable Observer OceanTheme " \
 "OctetSeqHelper OctetSeqHolder Oid OMGVMCID OpenDataException OpenMBeanAttributeInfo " \
 "OpenMBeanAttributeInfoSupport OpenMBeanConstructorInfo OpenMBeanConstructorInfoSupport " \
 "OpenMBeanInfo OpenMBeanInfoSupport OpenMBeanOperationInfo OpenMBeanOperationInfoSupport " \
 "OpenMBeanParameterInfo OpenMBeanParameterInfoSupport OpenType OpenType OperatingSystemMXBean " \
 "Operation OperationNotSupportedException OperationsException Option OptionalDataException " \
 "OptionPaneUI ORB ORB ORBIdHelper ORBInitializer ORBInitializerOperations ORBInitInfo " \
 "ORBInitInfoOperations OrientationRequested OutOfMemoryError OutputDeviceAssigned OutputKeys " \
 "OutputStream OutputStream OutputStream OutputStreamWriter OverlappingFileLockException " \
 "OverlayLayout Override Owner Pack200 Pack200.Packer Pack200.Unpacker Package PackedColorModel " \
 "Pageable PageAttributes PageAttributes.ColorType PageAttributes.MediaType " \
 "PageAttributes.OrientationRequestedType PageAttributes.OriginType PageAttributes.PrintQualityType " \
 "PagedResultsControl PagedResultsResponseControl PageFormat PageRanges PagesPerMinute " \
 "PagesPerMinuteColor Paint PaintContext PaintEvent Panel PanelUI Paper ParagraphView " \
 "Parameter ParameterBlock ParameterDescriptor ParameterizedType ParameterMetaData " \
 "ParameterMode ParameterModeHelper ParameterModeHolder ParseException ParsePosition " \
 "Parser Parser ParserAdapter ParserConfigurationException ParserDelegator ParserFactory " \
 "PartialResultException PasswordAuthentication PasswordCallback PasswordView Patch " \
 "PathIterator Pattern PatternSyntaxException PBEKey PBEKeySpec PBEParameterSpec " \
 "PDLOverrideSupported Permission Permission PermissionCollection Permissions " \
 "PERSIST_STORE PersistenceDelegate PersistentMBean PhantomReference Pipe Pipe.SinkChannel " \
 "Pipe.SourceChannel PipedInputStream PipedOutputStream PipedReader PipedWriter PixelGrabber " \
 "PixelInterleavedSampleModel PKCS8EncodedKeySpec PKIXBuilderParameters PKIXCertPathBuilderResult " \
 "PKIXCertPathChecker PKIXCertPathValidatorResult PKIXParameters PlainDocument PlainView " \
 "POA POAHelper POAManager POAManagerOperations POAOperations Point Point2D Point2D.Double " \
 "Point2D.Float PointerInfo Policy PolicyError PolicyErrorCodeHelper PolicyErrorHelper " \
 "PolicyErrorHolder PolicyFactory PolicyFactoryOperations PolicyHelper PolicyListHelper " \
 "PolicyListHolder PolicyNode PolicyOperations PolicyQualifierInfo PolicyTypeHelper " \
 "Polygon PooledConnection Popup PopupFactory PopupMenu PopupMenuEvent PopupMenuListener " \
 "PopupMenuUI Port Port.Info PortableRemoteObject PortableRemoteObjectDelegate " \
 "PortUnreachableException Position Position.Bias Predicate PreferenceChangeEvent " \
 "PreferenceChangeListener Preferences PreferencesFactory PreparedStatement " \
 "PresentationDirection Principal Principal PrincipalHolder Printable " \
 "PrinterAbortException PrinterException PrinterGraphics PrinterInfo " \
 "PrinterIOException PrinterIsAcceptingJobs PrinterJob PrinterLocation " \
 "PrinterMakeAndModel PrinterMessageFromOperator PrinterMoreInfo PrinterMoreInfoManufacturer " \
 "PrinterName PrinterResolution PrinterState PrinterStateReason PrinterStateReasons " \
 "PrinterURI PrintEvent PrintException PrintGraphics PrintJob PrintJobAdapter " \
 "PrintJobAttribute PrintJobAttributeEvent PrintJobAttributeListener PrintJobAttributeSet " \
 "PrintJobEvent PrintJobListener PrintQuality PrintRequestAttribute PrintRequestAttributeSet " \
 "PrintService PrintServiceAttribute PrintServiceAttributeEvent PrintServiceAttributeListener " \
 "PrintServiceAttributeSet PrintServiceLookup PrintStream PrintWriter PriorityBlockingQueue " \
 "PriorityQueue PRIVATE_MEMBER PrivateClassLoader PrivateCredentialPermission PrivateKey " \
 "PrivateMLet PrivilegedAction PrivilegedActionException PrivilegedExceptionAction Process " \
 "ProcessBuilder ProcessingInstruction ProfileDataException ProfileIdHelper ProgressBarUI " \
 "ProgressMonitor ProgressMonitorInputStream Properties PropertyChangeEvent PropertyChangeListener " \
 "PropertyChangeListenerProxy PropertyChangeSupport PropertyDescriptor PropertyEditor " \
 "PropertyEditorManager PropertyEditorSupport PropertyPermission PropertyResourceBundle " \
 "PropertyVetoException ProtectionDomain ProtocolException Provider Provider.Service " \
 "ProviderException Proxy Proxy.Type ProxySelector PSource PSource.PSpecified PSSParameterSpec " \
 "PUBLIC_MEMBER PublicKey PushbackInputStream PushbackReader QName QuadCurve2D " \
 "QuadCurve2D.Double QuadCurve2D.Float Query QueryEval QueryExp Queue QueuedJobCount " \
 "Random RandomAccess RandomAccessFile Raster RasterFormatException RasterOp " \
 "RC2ParameterSpec RC5ParameterSpec Rdn Readable ReadableByteChannel Reader " \
 "ReadOnlyBufferException ReadWriteLock RealmCallback RealmChoiceCallback REBIND " \
 "Receiver Rectangle Rectangle2D Rectangle2D.Double Rectangle2D.Float RectangularShape " \
 "ReentrantLock ReentrantReadWriteLock ReentrantReadWriteLock.ReadLock " \
 "ReentrantReadWriteLock.WriteLock Ref RefAddr Reference Reference Referenceable " \
 "ReferenceQueue ReferenceUriSchemesSupported ReferralException ReflectionException " \
 "ReflectPermission Refreshable RefreshFailedException Region RegisterableService " \
 "Registry RegistryHandler RejectedExecutionException RejectedExecutionHandler " \
 "Relation RelationException RelationNotFoundException RelationNotification " \
 "RelationService RelationServiceMBean RelationServiceNotRegisteredException " \
 "RelationSupport RelationSupportMBean RelationType RelationTypeNotFoundException " \
 "RelationTypeSupport RemarshalException Remote RemoteCall RemoteException RemoteObject " \
 "RemoteObjectInvocationHandler RemoteRef RemoteServer RemoteStub RenderableImage " \
 "RenderableImageOp RenderableImageProducer RenderContext RenderedImage RenderedImageFactory " \
 "Renderer RenderingHints RenderingHints.Key RepaintManager ReplicateScaleFilter " \
 "RepositoryIdHelper Request REQUEST_PROCESSING_POLICY_ID RequestInfo RequestInfoOperations " \
 "RequestingUserName RequestProcessingPolicy RequestProcessingPolicyOperations " \
 "RequestProcessingPolicyValue RequiredModelMBean RescaleOp ResolutionSyntax " \
 "Resolver ResolveResult ResourceBundle ResponseCache ResponseHandler Result " \
 "ResultSet ResultSetMetaData Retention RetentionPolicy ReverbType RGBImageFilter " \
 "RMIClassLoader RMIClassLoaderSpi RMIClientSocketFactory RMIConnection RMIConnectionImpl " \
 "RMIConnectionImpl_Stub RMIConnector RMIConnectorServer RMICustomMaxStreamFormat " \
 "RMIFailureHandler RMIIIOPServerImpl RMIJRMPServerImpl RMISecurityException " \
 "RMISecurityManager RMIServer RMIServerImpl RMIServerImpl_Stub RMIServerSocketFactory " \
 "RMISocketFactory Robot Role RoleInfo RoleInfoNotFoundException RoleList " \
 "RoleNotFoundException RoleResult RoleStatus RoleUnresolved RoleUnresolvedList " \
 "RootPaneContainer RootPaneUI RoundingMode RoundRectangle2D RoundRectangle2D.Double " \
 "RoundRectangle2D.Float RowMapper RowSet RowSetEvent RowSetInternal RowSetListener " \
 "RowSetMetaData RowSetMetaDataImpl RowSetReader RowSetWarning RowSetWriter RSAKey " \
 "RSAKeyGenParameterSpec RSAMultiPrimePrivateCrtKey RSAMultiPrimePrivateCrtKeySpec " \
 "RSAOtherPrimeInfo RSAPrivateCrtKey RSAPrivateCrtKeySpec RSAPrivateKey RSAPrivateKeySpec " \
 "RSAPublicKey RSAPublicKeySpec RTFEditorKit RuleBasedCollator Runnable Runtime RunTime " \
 "RuntimeErrorException RuntimeException RuntimeMBeanException RuntimeMXBean " \
 "RunTimeOperations RuntimeOperationsException RuntimePermission SampleModel Sasl " \
 "SaslClient SaslClientFactory SaslException SaslServer SaslServerFactory Savepoint " \
 "SAXException SAXNotRecognizedException SAXNotSupportedException SAXParseException " \
 "SAXParser SAXParserFactory SAXResult SAXSource SAXTransformerFactory Scanner " \
 "ScatteringByteChannel ScheduledExecutorService ScheduledFuture ScheduledThreadPoolExecutor " \
 "Schema SchemaFactory SchemaFactoryLoader SchemaViolationException Scrollable " \
 "Scrollbar ScrollBarUI ScrollPane ScrollPaneAdjustable ScrollPaneConstants " \
 "ScrollPaneLayout ScrollPaneLayout.UIResource ScrollPaneUI SealedObject SearchControls " \
 "SearchResult SecretKey SecretKeyFactory SecretKeyFactorySpi SecretKeySpec " \
 "SecureCacheResponse SecureClassLoader SecureRandom SecureRandomSpi Security " \
 "SecurityException SecurityManager SecurityPermission Segment SelectableChannel " \
 "SelectionKey Selector SelectorProvider Semaphore SeparatorUI Sequence SequenceInputStream " \
 "Sequencer Sequencer.SyncMode SerialArray SerialBlob SerialClob SerialDatalink " \
 "SerialException Serializable SerializablePermission SerialJavaObject SerialRef " \
 "SerialStruct Servant SERVANT_RETENTION_POLICY_ID ServantActivator ServantActivatorHelper " \
 "ServantActivatorOperations ServantActivatorPOA ServantAlreadyActive ServantAlreadyActiveHelper " \
 "ServantLocator ServantLocatorHelper ServantLocatorOperations ServantLocatorPOA " \
 "ServantManager ServantManagerOperations ServantNotActive ServantNotActiveHelper " \
 "ServantObject ServantRetentionPolicy ServantRetentionPolicyOperations " \
 "ServantRetentionPolicyValue ServerCloneException ServerError ServerException " \
 "ServerIdHelper ServerNotActiveException ServerRef ServerRequest ServerRequestInfo " \
 "ServerRequestInfoOperations ServerRequestInterceptor ServerRequestInterceptorOperations " \
 "ServerRuntimeException ServerSocket ServerSocketChannel ServerSocketFactory " \
 "ServiceContext ServiceContextHelper ServiceContextHolder ServiceContextListHelper " \
 "ServiceContextListHolder ServiceDetail ServiceDetailHelper ServiceIdHelper " \
 "ServiceInformation ServiceInformationHelper ServiceInformationHolder ServiceNotFoundException " \
 "ServicePermission ServiceRegistry ServiceRegistry.Filter ServiceUI ServiceUIFactory " \
 "ServiceUnavailableException Set SetOfIntegerSyntax SetOverrideType SetOverrideTypeHelper " \
 "Severity Shape ShapeGraphicAttribute SheetCollate Short ShortBuffer ShortBufferException " \
 "ShortHolder ShortLookupTable ShortMessage ShortSeqHelper ShortSeqHolder Sides Signature " \
 "SignatureException SignatureSpi SignedObject Signer SimpleAttributeSet SimpleBeanInfo " \
 "SimpleDateFormat SimpleDoc SimpleFormatter SimpleTimeZone SimpleType " \
 "SinglePixelPackedSampleModel SingleSelectionModel Size2DSyntax SizeLimitExceededException " \
 "SizeRequirements SizeSequence Skeleton SkeletonMismatchException SkeletonNotFoundException " \
 "SliderUI Socket SocketAddress SocketChannel SocketException SocketFactory SocketHandler " \
 "SocketImpl SocketImplFactory SocketOptions SocketPermission SocketSecurityException " \
 "SocketTimeoutException SoftBevelBorder SoftReference SortControl SortedMap SortedSet " \
 "SortingFocusTraversalPolicy SortKey SortResponseControl Soundbank SoundbankReader " \
 "SoundbankResource Source SourceDataLine SourceLocator SpinnerDateModel SpinnerListModel " \
 "SpinnerModel SpinnerNumberModel SpinnerUI SplitPaneUI Spring SpringLayout " \
 "SpringLayout.Constraints SQLData SQLException SQLInput SQLInputImpl SQLOutput " \
 "SQLOutputImpl SQLPermission SQLWarning SSLContext SSLContextSpi SSLEngine SSLEngineResult " \
 "SSLEngineResult.HandshakeStatus SSLEngineResult.Status SSLException SSLHandshakeException " \
 "SSLKeyException SSLPeerUnverifiedException SSLPermission SSLProtocolException " \
 "SslRMIClientSocketFactory SslRMIServerSocketFactory SSLServerSocket SSLServerSocketFactory " \
 "SSLSession SSLSessionBindingEvent SSLSessionBindingListener SSLSessionContext " \
 "SSLSocket SSLSocketFactory Stack StackOverflowError StackTraceElement StandardMBean " \
 "StartTlsRequest StartTlsResponse State StateEdit StateEditable StateFactory " \
 "Statement Streamable StreamableValue StreamCorruptedException StreamHandler " \
 "StreamPrintService StreamPrintServiceFactory StreamResult StreamSource StreamTokenizer " \
 "StrictMath String StringBuffer StringBufferInputStream StringBuilder StringCharacterIterator " \
 "StringContent StringHolder StringIndexOutOfBoundsException StringMonitor StringMonitorMBean " \
 "StringNameHelper StringReader StringRefAddr StringSelection StringSeqHelper StringSeqHolder " \
 "StringTokenizer StringValueExp StringValueHelper StringWriter Stroke Struct StructMember " \
 "StructMemberHelper Stub StubDelegate StubNotFoundException Style StyleConstants " \
 "StyleConstants.CharacterConstants StyleConstants.ColorConstants StyleConstants.FontConstants " \
 "StyleConstants.ParagraphConstants StyleContext StyledDocument StyledEditorKit " \
 "StyledEditorKit.AlignmentAction StyledEditorKit.BoldAction StyledEditorKit.FontFamilyAction " \
 "StyledEditorKit.FontSizeAction StyledEditorKit.ForegroundAction StyledEditorKit.ItalicAction " \
 "StyledEditorKit.StyledTextAction StyledEditorKit.UnderlineAction StyleSheet StyleSheet.BoxPainter " \
 "StyleSheet.ListPainter Subject SubjectDelegationPermission SubjectDomainCombiner " \
 "SUCCESSFUL SupportedValuesAttribute SuppressWarnings SwingConstants SwingPropertyChangeSupport " \
 "SwingUtilities SYNC_WITH_TRANSPORT SyncFactory SyncFactoryException SyncFailedException " \
 "SynchronousQueue SyncProvider SyncProviderException SyncResolver SyncScopeHelper " \
 "SynthConstants SynthContext Synthesizer SynthGraphicsUtils SynthLookAndFeel SynthPainter " \
 "SynthStyle SynthStyleFactory SysexMessage System SYSTEM_EXCEPTION SystemColor " \
 "SystemException SystemFlavorMap TabableView TabbedPaneUI TabExpander TableCellEditor " \
 "TableCellRenderer TableColumn TableColumnModel TableColumnModelEvent TableColumnModelListener " \
 "TableHeaderUI TableModel TableModelEvent TableModelListener TableUI TableView " \
 "TabSet TabStop TabularData TabularDataSupport TabularType TAG_ALTERNATE_IIOP_ADDRESS " \
 "TAG_CODE_SETS TAG_INTERNET_IOP TAG_JAVA_CODEBASE TAG_MULTIPLE_COMPONENTS TAG_ORB_TYPE " \
 "TAG_POLICIES TAG_RMI_CUSTOM_MAX_STREAM_FORMAT TagElement TaggedComponent " \
 "TaggedComponentHelper TaggedComponentHolder TaggedProfile TaggedProfileHelper " \
 "TaggedProfileHolder Target TargetDataLine TargetedNotification TCKind Templates " \
 "TemplatesHandler Text TextAction TextArea TextAttribute TextComponent TextEvent " \
 "TextField TextHitInfo TextInputCallback TextLayout TextLayout.CaretPolicy " \
 "TextListener TextMeasurer TextOutputCallback TextSyntax TextUI TexturePaint " \
 "Thread Thread.State Thread.UncaughtExceptionHandler THREAD_POLICY_ID ThreadDeath " \
 "ThreadFactory ThreadGroup ThreadInfo ThreadLocal ThreadMXBean ThreadPolicy " \
 "ThreadPolicyOperations ThreadPolicyValue ThreadPoolExecutor ThreadPoolExecutor.AbortPolicy " \
 "ThreadPoolExecutor.CallerRunsPolicy ThreadPoolExecutor.DiscardOldestPolicy " \
 "ThreadPoolExecutor.DiscardPolicy Throwable Tie TileObserver Time TimeLimitExceededException " \
 "TIMEOUT TimeoutException Timer TimerAlarmClockNotification TimerMBean TimerNotification " \
 "TimerTask Timestamp TimeUnit TimeZone TitledBorder ToolBarUI Toolkit ToolTipManager " \
 "ToolTipUI TooManyListenersException Track TRANSACTION_MODE TRANSACTION_REQUIRED " \
 "TRANSACTION_ROLLEDBACK TRANSACTION_UNAVAILABLE TransactionalWriter TransactionRequiredException " \
 "TransactionRolledbackException TransactionService Transferable TransferHandler " \
 "TransformAttribute Transformer TransformerConfigurationException TransformerException " \
 "TransformerFactory TransformerFactoryConfigurationError TransformerHandler TRANSIENT " \
 "Transmitter Transparency TRANSPORT_RETRY TreeCellEditor TreeCellRenderer " \
 "TreeExpansionEvent TreeExpansionListener TreeMap TreeModel TreeModelEvent " \
 "TreeModelListener TreeNode TreePath TreeSelectionEvent TreeSelectionListener " \
 "TreeSelectionModel TreeSet TreeUI TreeWillExpandListener TrustAnchor TrustManager " \
 "TrustManagerFactory TrustManagerFactorySpi Type TypeCode TypeCodeHolder TypeInfo " \
 "TypeInfoProvider TypeMismatch TypeMismatch TypeMismatch TypeMismatchHelper " \
 "TypeMismatchHelper TypeNotPresentException Types TypeVariable UID UIDefaults " \
 "UIDefaults.ActiveValue UIDefaults.LazyInputMap UIDefaults.LazyValue UIDefaults.ProxyLazyValue " \
 "UIEvent UIManager UIManager.LookAndFeelInfo UIResource ULongLongSeqHelper ULongLongSeqHolder " \
 "ULongSeqHelper ULongSeqHolder UndeclaredThrowableException UndoableEdit UndoableEditEvent " \
 "UndoableEditListener UndoableEditSupport UndoManager UnexpectedException UnicastRemoteObject " \
 "UnionMember UnionMemberHelper UNKNOWN UNKNOWN UnknownEncoding UnknownEncodingHelper " \
 "UnknownError UnknownException UnknownFormatConversionException UnknownFormatFlagsException " \
 "UnknownGroupException UnknownHostException UnknownHostException UnknownObjectException " \
 "UnknownServiceException UnknownUserException UnknownUserExceptionHelper " \
 "UnknownUserExceptionHolder UnmappableCharacterException UnmarshalException " \
 "UnmodifiableClassException UnmodifiableSetException UnrecoverableEntryException " \
 "UnrecoverableKeyException Unreferenced UnresolvedAddressException UnresolvedPermission " \
 "UnsatisfiedLinkError UnsolicitedNotification UnsolicitedNotificationEvent " \
 "UnsolicitedNotificationListener UNSUPPORTED_POLICY UNSUPPORTED_POLICY_VALUE " \
 "UnsupportedAddressTypeException UnsupportedAudioFileException UnsupportedCallbackException " \
 "UnsupportedCharsetException UnsupportedClassVersionError UnsupportedEncodingException " \
 "UnsupportedFlavorException UnsupportedLookAndFeelException UnsupportedOperationException " \
 "URI URIException URIResolver URISyntax URISyntaxException URL URLClassLoader " \
 "URLConnection URLDecoder URLEncoder URLStreamHandler URLStreamHandlerFactory " \
 "URLStringHelper USER_EXCEPTION UserDataHandler UserException UShortSeqHelper " \
 "UShortSeqHolder UTFDataFormatException Util UtilDelegate Utilities UUID Validator " \
 "ValidatorHandler ValueBase ValueBaseHelper ValueBaseHolder ValueExp ValueFactory " \
 "ValueHandler ValueHandlerMultiFormat ValueInputStream ValueMember ValueMemberHelper " \
 "ValueOutputStream VariableHeightLayoutCache Vector VerifyError VersionSpecHelper " \
 "VetoableChangeListener VetoableChangeListenerProxy VetoableChangeSupport View " \
 "ViewFactory ViewportLayout ViewportUI VirtualMachineError Visibility VisibilityHelper " \
 "VM_ABSTRACT VM_CUSTOM VM_NONE VM_TRUNCATABLE VMID VoiceStatus Void VolatileImage " \
 "WCharSeqHelper WCharSeqHolder WeakHashMap WeakReference WebRowSet WildcardType " \
 "Window WindowAdapter WindowConstants WindowEvent WindowFocusListener WindowListener " \
 "WindowStateListener WrappedPlainView WritableByteChannel WritableRaster " \
 "WritableRenderedImage WriteAbortedException Writer WrongAdapter WrongAdapterHelper " \
 "WrongPolicy WrongPolicyHelper WrongTransaction WrongTransactionHelper " \
 "WrongTransactionHolder WStringSeqHelper WStringSeqHolder WStringValueHelper " \
 "X500Principal X500PrivateCredential X509Certificate X509Certificate X509CertSelector " \
 "X509CRL X509CRLEntry X509CRLSelector X509EncodedKeySpec X509ExtendedKeyManager " \
 "X509Extension X509KeyManager X509TrustManager XAConnection XADataSource XAException " \
 "XAResource Xid XMLConstants XMLDecoder XMLEncoder XMLFilter XMLFilterImpl " \
 "XMLFormatter XMLGregorianCalendar XMLParseException XmlReader XMLReader XMLReaderAdapter " \
 "XMLReaderFactory XmlWriter XPath XPathConstants XPathException XPathExpression " \
 "XPathExpressionException XPathFactory XPathFactoryConfigurationException XPathFunction " \
 "XPathFunctionException XPathFunctionResolver XPathVariableResolver ZipEntry " \
 "ZipException ZipFile ZipInputStream ZipOutputStream ZoneView"

#define PythonKeywords_solo \
  "and as assert break class continue def del elif else except exec finally " \
  "for from global if import in is lambda not or pass print raise return try " \
  "while with yield"

#define PerlKeywords_solo \
  "NULL __FILE__ __LINE__ __PACKAGE__ __DATA__ __END__ AUTOLOAD BEGIN UNITCHECK " \
  "CORE DESTROY END INIT CHECK abs accept alarm and atan2 bind binmode bless break " \
  "caller chdir chmod chomp chop chown chr chroot close closedir cmp connect continue " \
  "cos crypt dbmclose dbmopen default defined delete die do dump each else elsif " \
  "endgrent endhostent endnetent endprotoent endpwent endservent eof eq err eval " \
  "exec exists exit exp fcntl fileno flock for foreach fork format formline ge " \
  "getc getgrent getgrgid getgrnam gethostbyaddr gethostbyname gethostent getlogin " \
  "getnetbyaddr getnetbyname getnetent getpeername getpgrp getppid getpriority " \
  "getprotobyname getprotobynumber getprotoent getpwent getpwnam getpwuid " \
  "getservbyname getservbyport getservent getsockname getsockopt given glob " \
  "gmtime goto grep gt hex if index int ioctl join keys kill last lc lcfirst le " \
  "length link listen local localtime lock log lstat lt m map mkdir msgctl msgget " \
  "msgrcv msgsnd my ne next no not oct open opendir or ord our pack package pipe " \
  "pop pos print printf prototype push q qq qr quotemeta qw qx rand read readdir " \
  "readline readlink readpipe recv redo ref rename require reset return reverse " \
  "rewinddir rindex rmdir s say scalar seek seekdir select semctl semget semop " \
  "send setgrent sethostent setnetent setpgrp setpriority setprotoent setpwent " \
  "setservent setsockopt shift shmctl shmget shmread shmwrite shutdown sin " \
  "sleep socket socketpair sort splice split sprintf sqrt srand stat state " \
  "study sub substr symlink syscall sysopen sysread sysseek system syswrite " \
  "tell telldir tie tied time times tr truncate uc ucfirst umask undef unless " \
  "unlink unpack unshift untie until use utime values vec wait waitpid " \
  "wantarray warn when while write x xor y"

#define PHPKeywords_solo \
  "and or xor __FILE__ exception __LINE__ array as break case class const continue " \
  "declare default die do echo else elseif empty enddeclare endfor endforeach " \
  "endif endswitch endwhile eval exit extends for foreach function global if " \
  "include include_once isset list new print require require_once return static " \
  "switch unset use var while __FUNCTION__ __CLASS__ __METHOD__ final " \
  "php_user_filter interface implements instanceof public private protected " \
  "abstract clone try catch throw cfunction old_function this final __NAMESPACE__ " \
  "namespace goto __DIR__"

#define BashKeywords_solo \
  "! case do done elif else esac fi for function if in select then until " \
  "while time"

#define BatchKeywords_solo \
  "ADDUSERS ARP ASSOC ASSOCIAT ATTRIB BOOTCFG BROWSTAT CACLS CALL CD CHANGE CHKDSK CHKNTFS CHOICE CIPHER CleanMgr CLEARMEM CLIP CLS CLUSTER CMD COLOR COMP COMPACT COMPRESS CON2PRT CONVERT COPY CSCcmd CSVDE DATE DEFRAG DEL DELPROF DELTREE DevCon DIR DIRUSE DISKCOMP DISKCOPY DISKPART DNSSTAT DOSKEY DSACLs DSAdd DSGet DSQuery DSMod DSMove DSRM  ECHO ENDLOCAL ERASE EVENTCREATE EXIT EXPAND EXTRACT  FC FIND FINDSTR FOR FOR FOR FORFILES FORMAT FREEDISK FSUTIL FTP FTYPE  GLOBAL GOTO GPUPDATE  HELP  CACLS IF IFMEMBER IPCONFIG  KILL  LABEL LOCAL LOGEVENT LOGOFF LOGTIME  MAPISEND MBSAcli MEM MD MKLINK MODE MORE MOUNTVOL MOVE MOVEUSER MSG MSIEXEC MSINFO MSTSC MUNGE MV  NET NETDOM NETSH NETSVC NBTSTAT NETSTAT NOW NSLOOKUP NTBACKUP NTRIGHTS  OPENFILES  PATH PATHPING PAUSE PERMS PERFMON PING POPD PORTQRY POWERCFG PRINT PRNCNFG PRNMNGR PROMPT PsExec PsFile PsGetSid PsInfo PsKill PsList PsLoggedOn PsLogList PsPasswd PsService PsShutdown PsSuspend PUSHD  QGREP  RASDIAL RASPHONE RECOVER REG REGEDIT REGSVR32 REGINI REM REN REPLACE RD RMTSHARE ROBOCOPY ROUTE RUNAS RUNDLL32  SC SCHTASKS SCLIST SET SETLOCAL SETX SFC SHARE SHIFT SHORTCUT SHOWGRPS SHOWMBRS SHUTDOWN SLEEP SLMGR SOON SORT START SU SUBINACL SUBST SYSTEMINFO  TASKLIST TASKKILL TIME TIMEOUT TITLE TLIST TOUCH TRACERT TREE TYPE  USRSTAT  VER VERIFY VOL  WHERE WHOAMI WINDIFF WINMSD WINMSDP WMIC XCACLS XCOPY ::"

const char SQLMajorKeywords[] =
  "accessible add all alter analyze and as asc asensitive "
  "before between bigint binary blob both by "
  "call cascade case change char character check collate column condition connection constraint "
    "continue convert create cross current_date current_time current_timestamp current_user cursor "
  "database databases day_hour day_microsecond day_minute day_second dec decimal declare default "
    "delayed delete desc describe deterministic distinct distinctrow div double drop dual "
  "each else elseif enclosed escaped exists exit explain "
  "false fetch float float4 float8 for force foreign from fulltext "
  "goto grant group "
  "having high_priority hour_microsecond hour_minute hour_second "
  "if ignore in index infile inner inout insensitive insert int int1 int2 int3 int4 int8 integer "
    "interval into is iterate "
  "join "
  "key keys kill "
  "label leading leave left like limit linear lines load localtime localtimestamp lock long "
    "longblob longtext loop low_priority "
  "master_ssl_verify_server_cert match mediumblob mediumint mediumtext middleint minute_microsecond "
    "minute_second mod modifies "
  "natural not no_write_to_binlog null numeric "
  "on optimize option optionally or order out outer outfile "
  "precision primary procedure purge "
  "range read reads read_only read_write real references regexp release rename repeat replace "
    "require restrict return revoke right rlike "
  "schema schemas second_microsecond select sensitive separator set show smallint spatial specific "
    "sql sqlexception sqlstate sqlwarning sql_big_result sql_calc_found_rows sql_small_result ssl "
    "starting straight_join "
  "table terminated then tinyblob tinyint tinytext to trailing trigger true "
  "undo union unique unlock unsigned update upgrade usage use using utc_date utc_time utc_timestamp "
  "values varbinary varchar varcharacter varying "
  "when where while with write "
  "xor "
  "year_month "
  "zerofill";

const char SQLProcedureKeywords[] = // Not reserved words but intrinsic part of procedure definitions.
  "begin comment end";

const char SQLClientKeywords[] = // Definition of keywords only used by clients, not the server itself.
  "delimiter";

const char SQLUserKeywords[] = // Definition of own keywords, not used by MySQL.
  "edit";

/* keys are a list of C++ keywords */
const char CKeywords[] = CKeywords_solo;

const char CppKeywords[] = CKeywords_solo CppKeywords_solo;

const char ObjCKeywords[] = CKeywords_solo ObjCKeywords_solo;

const char ObjCppKeywords[] = CKeywords_solo CppKeywords_solo ObjCKeywords_solo;

/* header files are used by C/C++ and ObjC, there is no way of knowing what
 * type of header file it is, so colourize everything in .h files
 */
const char HKeywords[] = CKeywords_solo CppKeywords_solo ObjCKeywords_solo;

const char JKeywords[] = JavaKeywords_solo;

const char JClasses[] = JavaAllClasses_solo;

const char PythonKeywords[] = PythonKeywords_solo;

const char PerlKeywords[] = PerlKeywords_solo;

const char PHPKeywords[] = PHPKeywords_solo;

const char BashKeywords[] = BashKeywords_solo;

/* keys 2 are a list of the C standard Library, minus a few obscure headers */
const char CStdLibrary[] =  CStdLibrary_solo;
#endif

/*
Copyright (c) 2010, Michael Mullin <masmullin@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by Michael Mullin <masmullin@gmail.com>.
4. Neither the name of Michael Mullin nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MICHAEL MULLIN ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICHAEL MULLIN BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
