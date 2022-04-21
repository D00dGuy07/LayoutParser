Main
{
	<Frame()
		ID = "frame",
		HorizontalBias = 1/2, VerticalBias = 1/2, ZIndex = 1,
		Fill = #33CC22, Roundness = 10, Alpha = 0.5,

		Width = <ScaleSize(0.2)>,
		Height = <AspectSize(1)>,
		
		Constraints = [
			Top = <SpringConstraint() Target = "Window", TargetSide = "Top">,
			Bottom = <SpringConstraint() Target = "Window", TargetSide = "Bottom">,
			Left = <SpringConstraint() Target = "Window", TargetSide = "Left">,
			Right = <SpringConstraint() Target = "Window", TargetSide = "Right">
		]
	>
}