   M S B A M L   `   `   `  ����J  FRGBWLampControl, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null1clr-namespace:RGBWLampControlRGBWLampControl  5   	        RGBWLampControl.MainWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   /localclr-namespace:RGBWLampControl     5   	   -   5          �Name$  RGBW_Lamp_control_app��5   	    � Title$ RGBW Lamp control app��5   	   $	��600��6'   $	��800��64   
���   � � 
ResizeMode$ CanMinimize=�6S    � Icon$&  100px_RGB_color_wheel_36.svg.png��6l   .��5	      � ���	#FF1F1F1F.+�5
   
   �� -   5         ��Name$ button��5
      $��Connect��6!   $
��Left=�63   $��	72,45,0,0q�6N   $	��Top=�6a   $��75��6y   6�   �� 5   
   -   5         ��Name$ StatusBlock��5      $
��Left=�6)   $��
10,545,0,0q�6D   $
��Wrap=�6X   $	��Top=�6�   $	��201��6�   
���������6l   ��  �� Path$ Status� �� NotifyOnTargetUpdated . 6�   �� 5   
   -   5         ��Name$ 	HueSlider��5      $
��Left=�6$   $��
10,299,0,0q�6?   $	��Top=�6S   $	��542��6k   	 � Maximum$	 36000Q�6w   
 � SmallChange$
 1Q�6�   6�   �� 5   
   -   5        $ 	textBlock��5      $
��Left=�6'   $��557,299,0,0q�6B   $
��Wrap=�6W   $	��Top=�6�   
���������6k   ��  �� ElementName$ 	HueSlider��$ Value�6�   �� 5   
   -   5        $ SaturationSlider��5      $
��Left=�6+   $��
10,320,0,0q�6F   $	��Top=�6Z   $	��542��6r   $
	 1000Q�6~   $
 1Q�6�   6�   �� 5   
   -   5        $ textBlock_Copy��5      $
��Left=�6,   $��557,320,0,0q�6G   $
��Wrap=�6\   $	��Top=�6�   
���������6p   �� *Value+$ SaturationSlider��6�   �� 5   
   -   5        $ BrightnessSlider��5      $
��Left=�6+   $��
10,341,0,0q�6F   $	��Top=�6Z   $	��542��6r   $
	 1000Q�6~   $
 1Q�6�   6�   �� 5   
   -	   5        $ textBlock_Copy1��5      $
��Left=�6-   $��557,341,0,0q�6H   $
��Wrap=�6]   $	��Top=�6�   
���������6q   �� *Value+$ BrightnessSlider��6�   �� 5   
   -
   5        $ button1��5      $��Button��6"   $
��Left=�63   $��
461,97,0,0q�6N   $	��Top=�6b   $��75��6z   6�   5      5      