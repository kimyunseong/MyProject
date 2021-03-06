# 문제

방글라데시의 정당들은 자신의 세를 과시하기 위해 정기적인 동맹 휴업(파업)을 추진하는데, 

이 동맹 휴업은 경제에 상당한 피해를 끼칠 수 있다. 

이 문제에서는 각 당을 동맹 휴업 지수라고 부르는 h라는 양의 정수로 나타낼 수 있다고 하자.

이 동맹 휴업 지수는 한 동맹 휴업과 다음 동맹 휴업 사이의 기간을 날짜 수로 표시한 값이다.

세 개의 정당이 있다고 하자. 그리고 i번째의 동맹의 휴업지수를 hi라고 할 때, h1=3, h2=4, h3=8이라고 가정하자. 

N(N=14)일 동안의 세 당의 행보를 시뮬레이션 하면 다음과 같이 표시할 수 있다. 

시뮬레이션은 항상 일요일에 시작하며 금요일이나 토요일에는 동맹 휴업이 없다.

![이미지](../images/동맹휴업.png)

이 결과를 보면 14일 동안 정확하게 5번의 동맹 휴업(3,4,8,9,12일)이 있음을 알 수 있다.

6일은 금요일이기 때문에 동맹 휴업이 일어나지 않는다. 결국 2주 동안 근무 일수로 5일의 동맹 휴업이 실시된다.

몇 정당의 동맹 휴업 지수와, 어떤 정수 N이 주어졌을 때, N일 가운데 동맹 휴업으로 인해 일을 하지 않은 근무 일수를 계산하라.

## 입력조건

입력의 첫 번째 줄에는 한 개의 정수 N(7≤N≤3,650)이 입력되며 이는 시뮬레이션을 돌릴 기간(날 수)을 나타낸다. 

그 다음 줄에는 정당의 개수를 나타내는 정수 P(1≤P≤100)가 들어간다. 

그 다음부터 시작하는 P개의 줄 가운데 i번째 줄에는 i번째 정당의 동맹 휴업 지수를 나타내는 양의 정수 hi(7의 배수는 아님)가 들어있다.

## 출력조건

입력에 대해 손실된 근무 일수를 한 줄에 하나씩 출력한다.

입력예 : 14 3 3 4 8

출력예 : 5
